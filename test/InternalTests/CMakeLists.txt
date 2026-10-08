# ------ Tau's Internal Tests  ------
add_executable(
    TauInternalTests
    main.c
    test.c
    test.cpp

    # Tau's Death Tests
    DeathTests/test_string_macros.c
    DeathTests/test_string_macros.cpp
    DeathTests/test_assertion_macros_1.c
    DeathTests/test_assertion_macros_2.c
    DeathTests/test_assertion_macros_1.cpp
    DeathTests/test_assertion_macros_2.cpp
)

install(
    TARGETS TauInternalTests
    RUNTIME DESTINATION ${TAU_BIN_DIR}
)

target_include_directories(
    TauInternalTests 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# The Tau INTERFACE library
target_link_libraries(TauInternalTests Tau)

# Exercise the report as a CI consumer would, including deliberately failing tests.
find_package(Python3 QUIET COMPONENTS Interpreter)
if(Python3_Interpreter_FOUND)
    add_executable(TauXmlReportTests xml_report.c)
    target_link_libraries(TauXmlReportTests Tau)
    add_test(NAME TauXmlReport
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/verify_xml_report.py
                $<TARGET_FILE:TauXmlReportTests>)
endif()
