#include "Arrow/Arrow.h"

// int main() {
//     arrow_coloured_printf_(ARROW_COLOR_GREEN_, "Hello World, %d", cast(UInt64, 2));
//     return 0;
// }

ARROW_MAIN()

TEST(c, ForLoop) { 
    int count = 0;
    for(int i = 0; i<10000000; i++) {
        count ++;
    }
    REQUIRE_EQ(count, 10000000); 
}

TEST(c, REQUIRE_NE) { REQUIRE_NE(1, 2); }

TEST(c, REQUIRE_LT) { REQUIRE_LT(1, 2); } 

TEST(c, REQUIRE_LE) {
    REQUIRE_LE(1, 1);
    REQUIRE_LE(1, 2);
}

TEST(c, REQUIRE_GT) { REQUIRE_GT(2, 1); }

TEST(c, REQUIRE_GE) {
    REQUIRE_GE(1, 1);
    REQUIRE_GE(2, 1);
}

TEST(c, REQUIRE_STREQ) { REQUIRE_STREQ("foo", "foo"); }

TEST(c, REQUIRE_STRNE) { REQUIRE_STRNEQ("foo", "bar"); }

TEST(c, REQUIRE_STRNEQ) { REQUIRE_STRNEQ("foo", "foobar"); }

TEST(c, REQUIRE_STRNNE) { REQUIRE_STRNNEQ("foo", "barfoo", strlen("foo")); }

TEST(c, HelloWorld) { CHECK_EQ(1, 2); }

TEST(c, CHECK_NE) { CHECK_NE(1, 2); }

TEST(c, CHECK_LT) { CHECK_LT(1, 2); }

TEST(c, CHECK_LE) {
    CHECK_LE(1, 1);
    CHECK_LE(1, 2);
}

TEST(c, CHECK_GT) { CHECK_GT(2, 1); }

TEST(c, CHECK_GE) {
    CHECK_GE(1, 1);
    CHECK_GE(2, 1);
}

TEST(c, CHECK_STREQ) { CHECK_STREQ("foo", "foo"); }

TEST(c, CHECK_STRNE) { CHECK_STRNEQ("foo", "bar"); }

TEST(c, CHECK_STRNEQ) { CHECK_STRNEQ("foo", "foobar"); }

TEST(c, CHECK_STRNNE) { CHECK_STRNNEQ("foo", "barfoo", strlen("foo")); }

// 
// ================================== C11 VERSION ==================================
// 

TEST(c11, REQUIRE_EQ) { REQUIRE_EQ(1, 3); }

TEST(c11, REQUIRE_NE) { REQUIRE_NE(1, 1); }

TEST(c11, REQUIRE_LT) { REQUIRE_LT(1, 2); }

TEST(c11, REQUIRE_LE) {
    REQUIRE_LE(1, 1);
    REQUIRE_LE(1, 2);
}

TEST(c11, REQUIRE_GT) { REQUIRE_GT(2, 1); }

TEST(c11, REQUIRE_GE) {
    REQUIRE_GE(1, 1);
    REQUIRE_GE(2, 1);
}

TEST(c11, REQUIRE_STREQ) { REQUIRE_STREQ("foo", "foo"); }

TEST(c11, REQUIRE_STRNE) { REQUIRE_STRNEQ("foo", "bar"); }

TEST(c11, CHECK_EQ) { CHECK_EQ(1, 1); }

TEST(c11, CHECK_NE) { CHECK_NE(1, 2); }

TEST(c11, CHECK_LT) { CHECK_LT(1, 2); }

TEST(c11, CHECK_LE) {
    CHECK_LE(1, 1);
    CHECK_LE(1, 2);
}

TEST(c11, CHECK_GT) { CHECK_GT(2, 1); }

TEST(c11, CHECK_GE) {
    CHECK_GE(1, 1);
    CHECK_GE(2, 1);
}

TEST(c11, CHECK_STREQ) { CHECK_STREQ("foo", "foo"); }

TEST(c11, CHECK_STRNE) { CHECK_STRNEQ("foo", "bar"); }

TEST(c11, no_double_eval) {
    int i = 0;
    REQUIRE_EQ(i++, 0);
    REQUIRE_EQ(i, 1);
}
