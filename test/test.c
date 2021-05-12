#include <Arrow/UTest.h>

ARROW_MAIN()


TEST(c, ASSERT_EQ) { ASSERT_EQ(1, 1); }

TEST(c, ASSERT_NE) { ASSERT_NE(1, 2); }

TEST(c, ASSERT_LT) { ASSERT_LT(1, 2); } 

TEST(c, ASSERT_LE) {
  ASSERT_LE(1, 1);
  ASSERT_LE(1, 2);
}

TEST(c, ASSERT_GT) { ASSERT_GT(2, 1); }

TEST(c, ASSERT_GE) {
  ASSERT_GE(1, 1);
  ASSERT_GE(2, 1);
}

TEST(c, ASSERT_STREQ) { ASSERT_STREQ("foo", "foo"); }

TEST(c, ASSERT_STRNE) { ASSERT_STRNEQ("foo", "bar"); }

TEST(c, ASSERT_STRNEQ) { ASSERT_STRNEQ("foo", "foobar"); }

TEST(c, ASSERT_STRNNE) { ASSERT_STRNNEQ("foo", "barfoo", strlen("foo")); }

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

TEST(c11, ASSERT_EQ) { ASSERT_EQ(1, 1); }

TEST(c11, ASSERT_NE) { ASSERT_NE(1, 2); }

TEST(c11, ASSERT_LT) { ASSERT_LT(1, 2); }

TEST(c11, ASSERT_LE) {
  ASSERT_LE(1, 1);
  ASSERT_LE(1, 2);
}

TEST(c11, ASSERT_GT) { ASSERT_GT(2, 1); }

TEST(c11, ASSERT_GE) {
  ASSERT_GE(1, 1);
  ASSERT_GE(2, 1);
}

TEST(c11, ASSERT_STREQ) { ASSERT_STREQ("foo", "foo"); }

TEST(c11, ASSERT_STRNE) { ASSERT_STRNEQ("foo", "bar"); }

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
  ASSERT_EQ(i++, 0);
  ASSERT_EQ(i, 1);
}
