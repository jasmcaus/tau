#include <test.h>

#include <stdio.h>

TEST(Hello, Hello) {
    CHECK_EQ(1, 2);
}

int main(int argc, char** argv) {
    change(argc, argv);
}