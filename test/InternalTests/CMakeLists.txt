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