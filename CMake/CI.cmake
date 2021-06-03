###############################################################################
# Thorough check with recent compilers
###############################################################################

set(CLANG_CFLAGS "-std=c11                            \
    -Werror                                           \
    -Weverything                                      \
    -Wno-c++98-compat                                 \
    -Wno-c++98-compat-pedantic                        \
    -Wno-deprecated-declarations                      \
    -Wno-documentation-unknown-command                \
    -Wno-exit-time-destructors                        \
    -Wno-extra-semi-stmt                              \
    -Wno-padded                                       \
    -Wno-range-loop-analysis                          \
    -Wno-switch-enum -Wno-covered-switch-default      \
    -Wno-weak-vtables                                 \
")

set(GCC_CFLAGS "-std=c11                              \
    -pedantic                                         \
    -Werror                                           \
    --all-warnings                                    \
    --extra-warnings                                  \
    -W                                                \
    -WNSObject-attribute                              \
    -Wno-abi-tag                                      \
    -Waddress                                         \
    -Waddress-of-packed-member                        \
    -Wno-aggregate-return                             \
    -Waggressive-loop-optimizations                   \
    -Waligned-new=all                                 \
    -Wall                                             \
    -Walloc-zero                                      \
    -Walloca                                          \
    -Wanalyzer-double-fclose                          \
    -Wanalyzer-double-free                            \
    -Wanalyzer-exposure-through-output-file           \
    -Wanalyzer-file-leak                              \
    -Wanalyzer-free-of-non-heap                       \
    -Wanalyzer-malloc-leak                            \
    -Wanalyzer-mismatching-deallocation               \
    -Wanalyzer-null-argument                          \
    -Wanalyzer-null-dereference                       \
    -Wanalyzer-possible-null-argument                 \
    -Wanalyzer-possible-null-dereference              \
    -Wanalyzer-shift-count-negative                   \
    -Wanalyzer-shift-count-overflow                   \
    -Wanalyzer-stale-setjmp-buffer                    \
    -Wanalyzer-tainted-array-index                    \
    -Wanalyzer-too-complex                            \
    -Wanalyzer-unsafe-call-within-signal-handler      \
    -Wanalyzer-use-after-free                         \
    -Wanalyzer-use-of-pointer-in-stale-stack-frame    \
    -Wanalyzer-write-to-const                         \
    -Wanalyzer-write-to-string-literal                \
    -Warith-conversion                                \
    -Warray-bounds                                    \
    -Warray-bounds=2                                  \
    -Warray-parameter=2                               \
    -Wattribute-alias=2                               \
    -Wattribute-warning                               \
    -Wattributes                                      \
    -Wbool-compare                                    \
    -Wbool-operation                                  \
    -Wbuiltin-declaration-mismatch                    \
    -Wbuiltin-macro-redefined                         \
    -Wc++0x-compat                                    \
    -Wc++11-compat                                    \
    -Wc++14-compat                                    \
    -Wc++17-compat                                    \
    -Wc++1z-compat                                    \
    -Wc++20-compat                                    \
    -Wc++2a-compat                                    \
    -Wcannot-profile                                  \
    -Wcast-align                                      \
    -Wcast-align=strict                               \
    -Wcast-function-type                              \
    -Wcast-qual                                       \
    -Wcatch-value=3                                   \
    -Wchar-subscripts                                 \
    -Wclass-conversion                                \
    -Wclass-memaccess                                 \
    -Wclobbered                                       \
    -Wcomma-subscript                                 \
    -Wcomment                                         \
    -Wcomments                                        \
    -Wconditionally-supported                         \
    -Wconversion                                      \
    -Wconversion-null                                 \
    -Wcoverage-mismatch                               \
    -Wcpp                                             \
    -Wctad-maybe-unsupported                          \
    -Wctor-dtor-privacy                               \
    -Wdangling-else                                   \
    -Wdate-time                                       \
    -Wdelete-incomplete                               \
    -Wdelete-non-virtual-dtor                         \
    -Wdeprecated                                      \
    -Wdeprecated-copy                                 \
    -Wdeprecated-copy-dtor                            \
    -Wdeprecated-declarations                         \
    -Wdeprecated-enum-enum-conversion                 \
    -Wdeprecated-enum-float-conversion                \
    -Wdisabled-optimization                           \
    -Wdiv-by-zero                                     \
    -Wdouble-promotion                                \
    -Wduplicated-branches                             \
    -Wduplicated-cond                                 \
    -Weffc++                                          \
    -Wempty-body                                      \
    -Wendif-labels                                    \
    -Wenum-compare                                    \
    -Wenum-conversion                                 \
    -Wexpansion-to-defined                            \
    -Wextra                                           \
    -Wextra-semi                                      \
    -Wfloat-conversion                                \
    -Wfloat-equal                                     \
    -Wformat-diag                                     \
    -Wformat-overflow=2                               \
    -Wformat-signedness                               \
    -Wformat-truncation=2                             \
    -Wformat=2                                        \
    -Wframe-address                                   \
    -Wfree-nonheap-object                             \
    -Whsa                                             \
    -Wif-not-aligned                                  \
    -Wignored-attributes                              \
    -Wignored-qualifiers                              \
    -Wimplicit-fallthrough=5                          \
    -Winaccessible-base                               \
    -Winherited-variadic-ctor                         \
    -Winit-list-lifetime                              \
    -Winit-self                                       \
    -Winline                                          \
    -Wint-in-bool-context                             \
    -Wint-to-pointer-cast                             \
    -Winvalid-memory-model                            \
    -Winvalid-offsetof                                \
    -Winvalid-pch                                     \
    -Wliteral-suffix                                  \
    -Wlogical-not-parentheses                         \
    -Wlogical-op                                      \
    -Wno-long-long                                    \
    -Wlto-type-mismatch                               \
    -Wmain                                            \
    -Wmaybe-uninitialized                             \
    -Wmemset-elt-size                                 \
    -Wmemset-transposed-args                          \
    -Wmisleading-indentation                          \
    -Wmismatched-dealloc                              \
    -Wmismatched-new-delete                           \
    -Wmismatched-tags                                 \
    -Wmissing-attributes                              \
    -Wmissing-braces                                  \
    -Wmissing-declarations                            \
    -Wmissing-field-initializers                      \
    -Wmissing-include-dirs                            \
    -Wmissing-profile                                 \
    -Wmultichar                                       \
    -Wmultiple-inheritance                            \
    -Wmultistatement-macros                           \
    -Wno-namespaces                                   \
    -Wnarrowing                                       \
    -Wno-noexcept                                     \
    -Wnoexcept-type                                   \
    -Wnon-template-friend                             \
    -Wnon-virtual-dtor                                \
    -Wnonnull                                         \
    -Wnonnull-compare                                 \
    -Wnormalized=nfkc                                 \
    -Wnull-dereference                                \
    -Wodr                                             \
    -Wold-style-cast                                  \
    -Wopenmp-simd                                     \
    -Woverflow                                        \
    -Woverlength-strings                              \
    -Woverloaded-virtual                              \
    -Wpacked                                          \
    -Wpacked-bitfield-compat                          \
    -Wpacked-not-aligned                              \
    -Wno-padded                                       \
    -Wparentheses                                     \
    -Wpedantic                                        \
    -Wpessimizing-move                                \
    -Wplacement-new=2                                 \
    -Wpmf-conversions                                 \
    -Wpointer-arith                                   \
    -Wpointer-compare                                 \
    -Wpragmas                                         \
    -Wprio-ctor-dtor                                  \
    -Wpsabi                                           \
    -Wno-range-loop-construct                         \
    -Wredundant-decls                                 \
    -Wredundant-move                                  \
    -Wredundant-tags                                  \
    -Wregister                                        \
    -Wreorder                                         \
    -Wrestrict                                        \
    -Wreturn-local-addr                               \
    -Wreturn-type                                     \
    -Wscalar-storage-order                            \
    -Wsequence-point                                  \
    -Wshadow=compatible-local                         \
    -Wshadow=global                                   \
    -Wshadow=local                                    \
    -Wshift-count-negative                            \
    -Wshift-count-overflow                            \
    -Wshift-negative-value                            \
    -Wshift-overflow=2                                \
    -Wsign-compare                                    \
    -Wsign-conversion                                 \
    -Wsign-promo                                      \
    -Wsized-deallocation                              \
    -Wsizeof-array-argument                           \
    -Wsizeof-array-div                                \
    -Wsizeof-pointer-div                              \
    -Wsizeof-pointer-memaccess                        \
    -Wstack-protector                                 \
    -Wstrict-aliasing                                 \
    -Wstrict-aliasing=3                               \
    -Wstrict-null-sentinel                            \
    -Wstrict-overflow                                 \
    -Wstrict-overflow=5                               \
    -Wstring-compare                                  \
    -Wstringop-overflow=4                             \
    -Wstringop-overread                               \
    -Wstringop-truncation                             \
    -Wsubobject-linkage                               \
    -Wsuggest-attribute=cold                          \
    -Wsuggest-attribute=const                         \
    -Wsuggest-attribute=format                        \
    -Wsuggest-attribute=malloc                        \
    -Wsuggest-attribute=noreturn                      \
    -Wsuggest-attribute=pure                          \
    -Wsuggest-final-methods                           \
    -Wsuggest-final-types                             \
    -Wsuggest-override                                \
    -Wswitch                                          \
    -Wswitch-bool                                     \
    -Wswitch-default                                  \
    -Wno-switch-enum                                  \
    -Wswitch-outside-range                            \
    -Wswitch-unreachable                              \
    -Wsync-nand                                       \
    -Wsynth                                           \
    -Wno-system-headers                               \
    -Wtautological-compare                            \
    -Wno-templates                                    \
    -Wterminate                                       \
    -Wtrampolines                                     \
    -Wtrigraphs                                       \
    -Wtsan                                            \
    -Wtype-limits                                     \
    -Wundef                                           \
    -Wuninitialized                                   \
    -Wunknown-pragmas                                 \
    -Wunreachable-code                                \
    -Wunsafe-loop-optimizations                       \
    -Wunused                                          \
    -Wunused-but-set-parameter                        \
    -Wunused-but-set-variable                         \
    -Wunused-const-variable=2                         \
    -Wunused-function                                 \
    -Wunused-label                                    \
    -Wunused-local-typedefs                           \
    -Wunused-macros                                   \
    -Wunused-parameter                                \
    -Wunused-result                                   \
    -Wunused-value                                    \
    -Wunused-variable                                 \
    -Wno-useless-cast                                 \
    -Wvarargs                                         \
    -Wvariadic-macros                                 \
    -Wvector-operation-performance                    \
    -Wvexing-parse                                    \
    -Wvirtual-inheritance                             \
    -Wvirtual-move-assign                             \
    -Wvla                                             \
    -Wvla-parameter                                   \
    -Wvolatile                                        \
    -Wvolatile-register-var                           \
    -Wwrite-strings                                   \
    -Wzero-as-null-pointer-constant                   \
    -Wzero-length-bounds                              \
")

add_custom_target(CI_TEST_GCC
    COMMAND C=${GCC_TOOL} CFLAGS=${GCC_CFLAGS} 
        ${CMAKE_COMMAND}
        -D CMAKE_BUILD_TYPE=Debug 
        -G "MinGW Makefiles"
        -D TAU_BUILDINTERNALTESTS=ON
        -S ${PROJECT_SOURCE_DIR} -B ${PROJECT_BINARY_DIR}/build_gcc
    COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/build_gcc
    COMMAND cd ${PROJECT_BINARY_DIR}/build_gcc && ${CMAKE_CTEST_COMMAND} --parallel ${N} --output-on-failure
    COMMENT "Compile and test with GCC using maximum warning flags"
)

add_custom_target(CI_TEST_CLANG
    COMMAND C=${CLANG_TOOL} CFLAGS=${CLANG_CFLAGS} 
        ${CMAKE_COMMAND}
        -D CMAKE_BUILD_TYPE=Debug 
        -G "MinGW Makefiles"
        -D TAU_BUILDINTERNALTESTS=ON
        -S ${PROJECT_SOURCE_DIR} -B ${PROJECT_BINARY_DIR}/build_clang
    COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/build_clang
    COMMAND cd ${PROJECT_BINARY_DIR}/build_clang && ${CMAKE_CTEST_COMMAND} --parallel ${N} --output-on-failure
    COMMENT "Compile and test with Clang using maximum warning flags"
)

###############################################################################
# Clean up all generated files.
###############################################################################

add_custom_target(ci_clean
    COMMAND rm -fr ${PROJECT_BINARY_DIR}/build_* cmake-3.1.0-Darwin64 ${JSON_CMAKE_FLAG_BUILD_DIRS} ${single_binaries}
    COMMENT "Clean generated directories"
)