// #include <stdio.h>


// // Here's one way to do it. It uses the list of arguments twice, first to form the name of the helper macro, and then to pass the arguments to that helper macro. It uses a standard trick to count the number of arguments to a macro.
// // SOURCE: https://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros
// // and: https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
// enum
// {
//     plain = 0,
//     bold = 1,
//     italic = 2
// };

// void PrintString(const char* message, int size, int style)
// {
//     printf("size = %d; style = %d, message = %s\n", size, style, message);
// }

// #define PRINT_STRING_1_ARGS(cond)              CHECK(cond, "Failure")
// #define PRINT_STRING_2_ARGS(cond, size)        CHECK(cond, message)
// // #define PRINT_STRING_3_ARGS(message, size, style) PRINTER(message, size, style)

// #define GET_4TH_ARG(arg1, arg2, arg3, arg4, ...) arg4
// #define MACRO_CHOOSER(...) \
//     GET_4TH_ARG(__VA_ARGS__, PRINT_STRING_2_ARGS, PRINT_STRING_1_ARGS, )
// // #define MACRO_CHOOSER(...) \
// //     GET_4TH_ARG(__VA_ARGS__, PRINT_STRING_3_ARGS, PRINT_STRING_2_ARGS, PRINT_STRING_1_ARGS, )

// #define PRINT_STRING(...) MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

// int main(int argc, char * const argv[])
// {
//     PRINT_STRING(1 != 1, "hello", "hdhd");

//     return 0;
// }

// // int main() {
// //     create();
// //     create(10);
// //     create(10, 10);
// // }

#include <test.h>

TEST(Hello, Hello) {
    CHECK_EQ(1, 2);
}

TEST(Hello, Hello2) {
    CHECK_EQ(2, 2);
}