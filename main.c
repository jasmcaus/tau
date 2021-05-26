#include <test.h>

#include <stdio.h>
MUON_MAIN()

TEST(Hello, Hello) {
    CHECK_EQ(1, 2);
}

TEST(Hello, Hello2) {
    CHECK_EQ(2, 2);
}

// int main(int argc, char** argv) {
//     change(argc, argv);
// }