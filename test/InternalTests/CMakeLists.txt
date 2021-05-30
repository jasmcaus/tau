# ------ Mhon's Internal Tests  ------
add_executable(
    MuonInternalTests
    main.c
    test.c
    test.cpp

    # Muon's Death Tests
    DeathTests/test_string_macros.c
    DeathTests/test_string_macros.cpp
    DeathTests/test_assertion_macros.c
    DeathTests/test_assertion_macros.cpp
)

install(
    TARGETS MuonInternalTests
    RUNTIME DESTINATION ${MUON_BIN_DIR}
)

target_include_directories(
    MuonInternalTests 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# The Muon INTERFACE library
target_link_libraries(MuonInternalTests Muon)