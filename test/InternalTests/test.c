#include "tau/tau.h"

TEST(c, CHECK_TF) {
    CHECK_FALSE(0);
    CHECK_TRUE(1);
}

TEST(c, REQUIRE_TF) {
    REQUIRE_FALSE(0);
    REQUIRE_TRUE(1);
}

TEST(c, MemAlloc) {
    void* mem;
    
    mem = malloc(5);
    CHECK(mem != NULL, "Mem ideally should not be NULL");

    mem = realloc(mem, 10);
    CHECK(mem != NULL, "Mem ideally should not be NULL");
}

int retu() {
    return 10000000;
}
TEST(c, ForLoop) { 
    int count = 0;
    for(int i = 0; i<10000000; i++) {
        count++;
    }
    REQUIRE_EQ(count, retu()); 
}

TEST(c, CHECK) { 
    CHECK(1 < 2, "1 < 2"); 
}

TEST(c, REQUIRE_LT) { 
    REQUIRE_LT(1, 2);  
}

TEST(c, REQUIRE_LE) {
    REQUIRE_LE(1, 1);
    REQUIRE_LE(1, 2);
}

TEST(c, REQUIRE_GT) { 
    REQUIRE_GT(2, 1); 
}

TEST(c, REQUIRE_GE) {
    REQUIRE_GE(1, 1);
    REQUIRE_GE(2, 1);
}

TEST(c, REQUIRE_STREQ) { 
    REQUIRE_STREQ("foo", "foo"); 
}

TEST(c, REQUIRE_STRNE) { 
    REQUIRE_STRNE("foo", "bar"); 
}

TEST(c, REQUIRE_SUBSTREQ) { 
    REQUIRE_SUBSTREQ("fooder", "foobar", strlen("foo")); 
}

TEST(c, REQUIRE_SUBSTRNE) { 
    REQUIRE_SUBSTRNE("foo", "barfoo", strlen("foo")); 
}

TEST(c, CHECK_EQ1) { 
    CHECK_EQ(345, 345); 
}

TEST(c, CHECK_NE) { 
    CHECK_NE(1, 2); 
}

TEST(c, CHECK_LT) { 
    CHECK_LT(1, 2); 
}

TEST(c, CHECK_LE) {
    CHECK_LE(1, 1);
    CHECK_LE(1, 2);
}

TEST(c, CHECK_GT) { CHECK_GT(2, 1); }

TEST(c, CHECK_GE) {
    CHECK_GE(1, 1);
    CHECK_GE(2, 1);
}

TEST(c, CHECK_STREQ) { 
    CHECK_STREQ("foo", "foo"); 
}

TEST(c, CHECK_STRNE) { 
    CHECK_STRNE("foo", "bar"); 
}

TEST(c, CHECK_SUBSTREQ) { 
    CHECK_SUBSTREQ("foo", "foobar", strlen("foo")); 
}

TEST(c, CHECK_SUBSTRNE) { 
    CHECK_SUBSTRNE("foo", "barfoo", strlen("foo")); 
}

TEST(c11, REQUIRE_EQ) { 
    REQUIRE_EQ(1, 1); 
}

TEST(c11, REQUIRE_NE) { 
    REQUIRE_NE(1, 2); 
}

TEST(c11, REQUIRE_LT) { 
    REQUIRE_LT(1, 2); 
}

TEST(c11, REQUIRE_LE) {
    REQUIRE_LE(1, 1);
    REQUIRE_LE(1, 2);
}

TEST(c11, REQUIRE_GT) { 
    REQUIRE_GT(2, 1); 
}

TEST(c11, REQUIRE_GE) {
    REQUIRE_GE(1, 1);
    REQUIRE_GE(2, 1);
}

TEST(c11, REQUIRE_STREQ) { 
    REQUIRE_STREQ("foo", "foo"); 
}

TEST(c11, REQUIRE_SUBSTREQ) { 
    REQUIRE_STRNE("foo", "bar"); 
}

TEST(c11, CHECK_EQ2) { 
    CHECK_EQ(1, 1); 
}

TEST(c11, CHECK_NE) { 
    CHECK_NE(1, 2); 
}

TEST(c11, CHECK_LT) { 
    CHECK_LT(1, 2); 
}

TEST(c11, CHECK_LE) {
    CHECK_LE(1, 1);
    CHECK_LE(1, 2);
}

TEST(c11, CHECK_GT) { 
    CHECK_GT(3, 1); 
}

TEST(c11, CHECK_GE) {
    CHECK_GE(1, 1);
    CHECK_GE(2, 1);
}

TEST(c11, CHECK_STREQ) { 
    CHECK_STREQ("foo", "foo"); 
}

TEST(c11, CHECK_SUBSTREQ) { 
    CHECK_STRNE("foo", "bar"); 
}

TEST(c11, no_double_eval) {
    int i = 0;
    REQUIRE_EQ(i++, 0);
    REQUIRE_EQ(i, 1);
}

TEST(c11, CHECK_BUF_EQ) {
	TAU_UInt8 buf[] = {1, 2, 3, 4, 5};
	TAU_UInt8 ref[] = {1, 2, 3, 4, 5};
	CHECK_BUF_EQ(buf, ref, sizeof(ref));
}

TEST(c11, CHECK_BUF_NE) {
	TAU_UInt8 buf[] = {1, 2, 3, 4, 5};
	TAU_UInt8 ref[] = {10, 2, 3, 4, 5};
	CHECK_BUF_NE(buf, ref, sizeof(ref));
}

TEST(c11, REQUIRE_BUF_EQ) {
	TAU_UInt8 buf[] = {1, 2, 3, 4, 5};
	TAU_UInt8 ref[] = {1, 2, 3, 4, 5};
	REQUIRE_BUF_EQ(buf, ref, sizeof(ref));
}

TEST(c11, REQUIRE_BUF_NE) {
	TAU_UInt8 buf[] = {1, 2, 3, 4, 5};
	TAU_UInt8 ref[] = {10, 23, 3, 4, 5};
	REQUIRE_BUF_NE(buf, ref, sizeof(ref));
}

struct MyTestF {
  int foo;
};

TEST_F_SETUP(MyTestF) {
    REQUIRE_EQ(0, tau->foo);
    tau->foo = 42;
}

TEST_F_TEARDOWN(MyTestF) { 
    REQUIRE_EQ(13, tau->foo); 
}

TEST_F(MyTestF, c) {
    REQUIRE_EQ(42, tau->foo);
    tau->foo = 13;
}

TEST_F(MyTestF, c2) {
    REQUIRE_EQ(42, tau->foo);
    tau->foo = 13;
}