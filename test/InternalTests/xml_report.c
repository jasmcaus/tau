#include <tau/tau.h>

TAU_MAIN()

TEST(report, passes) {
    CHECK_EQ(1, 1);
}

TEST(report, check_fails) {
    CHECK_EQ(1, 2);
    CHECK_EQ(3, 4);
}

TEST(report, require_fails) {
    REQUIRE_EQ(1, 2);
}
