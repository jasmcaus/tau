"""Verify XML reports identify failed tests, rather than reporting them as passes."""

import pathlib
import subprocess
import sys
import tempfile
import xml.etree.ElementTree as ET


with tempfile.TemporaryDirectory() as directory:
    report = pathlib.Path(directory) / "report.xml"
    result = subprocess.run(
        [sys.argv[1], "--output=" + str(report)],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        timeout=30,
    )
    assert result.returncode == 2, result.stdout
    cases = ET.parse(report).findall(".//testcase")
    assert len(cases) == 3, "Report should contain all three test cases"
    by_name = {case.attrib["name"]: case for case in cases}
    assert by_name["report.passes"].find("failure") is None
    for name in ("report.check_fails", "report.require_fails"):
        failures = by_name[name].findall("failure")
        assert len(failures) == 1, name + " must have exactly one failure element"
