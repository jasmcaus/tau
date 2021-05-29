# ------ Mhon's Internal Tests  ------
set(MUON_INTERNALTESTS    "MuonInternalTests")
add_executable(
    ${MUON_INTERNALTESTS}
    main.c
    test.c
    test.cpp

    # Muon's Death Tests
    DeathTests/test_string_macros.c
    DeathTests/test_string_macros.cpp
    # DeathTests/test_assertion_macros.c
    # DeathTests/test_assertion_macros.cpp
)

target_include_directories(
    ${MUON_INTERNALTESTS} 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# The Muon INTERFACE library
target_link_libraries(${MUON_INTERNALTESTS} Muon)