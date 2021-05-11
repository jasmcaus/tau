#include <Arrow/UTest.h>

ARROW_MAIN()


TEST(c, ASSERT_TRUE) { ASSERT_TRUE(1); }

TEST(c, ASSERT_FALSE) { ASSERT_FALSE(0); }

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

TEST(c, ASSERT_STRNE) { ASSERT_STRNE("foo", "bar"); }

TEST(c, ASSERT_STRNEQ) { ASSERT_STRNEQ("foo", "foobar", strlen("foo")); }

TEST(c, ASSERT_STRNNE) { ASSERT_STRNNE("foo", "barfoo", strlen("foo")); }

TEST(c, EXPECT_TRUE) { EXPECT_TRUE(1); }

TEST(c, EXPECT_FALSE) { EXPECT_FALSE(0); }

TEST(c, EXPECT_EQ) { EXPECT_EQ(1, 1); }

TEST(c, EXPECT_NE) { EXPECT_NE(1, 2); }

TEST(c, EXPECT_LT) { EXPECT_LT(1, 2); }

TEST(c, EXPECT_LE) {
  EXPECT_LE(1, 1);
  EXPECT_LE(1, 2);
}

TEST(c, EXPECT_GT) { EXPECT_GT(2, 1); }

TEST(c, EXPECT_GE) {
  EXPECT_GE(1, 1);
  EXPECT_GE(2, 1);
}

TEST(c, EXPECT_STREQ) { EXPECT_STREQ("foo", "foo"); }

TEST(c, EXPECT_STRNE) { EXPECT_STRNE("foo", "bar"); }

TEST(c, EXPECT_STRNEQ) { EXPECT_STRNEQ("foo", "foobar", strlen("foo")); }

TEST(c, EXPECT_STRNNE) { EXPECT_STRNNE("foo", "barfoo", strlen("foo")); }

// 
// ================================== C11 VERSION ==================================
// 

TEST(c11, ASSERT_TRUE) { ASSERT_TRUE(1); }

TEST(c11, ASSERT_FALSE) { ASSERT_FALSE(0); }

TEST(c11, ASSERT_EQ) { ASSERT_EQ(1, 1); }

TEST(c11, ASSERT_NE) { ASSERT_NE(1, 2); }

TEST(c11, ASSERT_LT) { ASSERT_LT(1, 0); }

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

TEST(c11, ASSERT_STRNE) { ASSERT_STRNE("foo", "bar"); }

TEST(c11, EXPECT_TRUE) { EXPECT_TRUE(1); }

TEST(c11, EXPECT_FALSE) { EXPECT_FALSE(0); }

TEST(c11, EXPECT_EQ) { EXPECT_EQ(1, 1); }

TEST(c11, EXPECT_NE) { EXPECT_NE(1, 2); }

TEST(c11, EXPECT_LT) { EXPECT_LT(1, 2); }

TEST(c11, EXPECT_LE) {
  EXPECT_LE(1, 1);
  EXPECT_LE(1, 2);
}

TEST(c11, EXPECT_GT) { EXPECT_GT(2, 1); }

TEST(c11, EXPECT_GE) {
  EXPECT_GE(1, 1);
  EXPECT_GE(2, 1);
}

TEST(c11, EXPECT_STREQ) { EXPECT_STREQ("foo", "foo"); }

TEST(c11, EXPECT_STRNE) { EXPECT_STRNE("foo", "bar"); }

TEST(c11, no_double_eval) {
  int i = 0;
  ASSERT_EQ(i++, 0);
  ASSERT_EQ(i, 1);
}
