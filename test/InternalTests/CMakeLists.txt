# ------ Mhon's Internal Tests  ------
set(MUON_INTERNALTESTS    "MuonInternalTests")
add_executable(
    ${MUON_INTERNALTESTS}
    main.c 
    # Do not include the following as they are pulled in automatically as if they were header files
    # On MSVC, this results in `error LNK2005` - this makes sense because we are compiling test.c and test.cpp twice?
    # test.c
    # test.cpp
)

target_include_directories(
    ${MUON_INTERNALTESTS} 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# The Muon INTERFACE library
target_link_libraries(${MUON_INTERNALTESTS} Muon)