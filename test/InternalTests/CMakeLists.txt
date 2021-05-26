# ------ Mhon's Internal Tests  ------
set(MUON_INTERNALTESTS    "MuonInternalTests")
add_executable(
    ${MUON_INTERNALTESTS}
    main.c 
    test.c
    # test.cpp
)

target_include_directories(
    ${MUON_INTERNALTESTS} 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# The Muon INTERFACE library
target_link_libraries(${MUON_INTERNALTESTS} Muon)