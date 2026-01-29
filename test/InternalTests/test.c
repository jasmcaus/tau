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

int retu(void) {
    return 10000000;
}
TEST(c, ForLoop)
{
    int count = 0;
    for(int i = 0; i<10000000; i++) {
        count++;
    }
    REQUIRE_EQ(count, retu());
}

TEST(c, CHECK)
{
    CHECK(1 < 2, "1 < 2");
}

TEST(c, REQUIRE_LT)
{
    REQUIRE_LT(1, 2);
}

TEST(c, REQUIRE_LE) {
    REQUIRE_LE(1, 1);
    REQUIRE_LE(1, 2);
}

TEST(c, REQUIRE_GT)
{
    REQUIRE_GT(2, 1);
}

TEST(c, REQUIRE_GE) {
    REQUIRE_GE(1, 1);
    REQUIRE_GE(2, 1);
}

TEST(c, REQUIRE_STREQ)
{
    REQUIRE_STREQ("foo", "foo");
}

TEST(c, REQUIRE_STRNE)
{
    REQUIRE_STRNE("foo", "bar");
}

TEST(c, REQUIRE_SUBSTREQ)
{
    REQUIRE_SUBSTREQ("fooder", "foobar", strlen("foo"));
}

TEST(c, REQUIRE_SUBSTRNE)
{
    REQUIRE_SUBSTRNE("foo", "barfoo", strlen("foo"));
}

TEST(c, CHECK_EQ1)
{
    CHECK_EQ(345, 345);
}

TEST(c, CHECK_NE)
{
    CHECK_NE(1, 2);
}

TEST(c, CHECK_LT)
{
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

TEST(c, CHECK_STREQ)
{
    CHECK_STREQ("foo", "foo");
}

TEST(c, CHECK_STRNE)
{
    CHECK_STRNE("foo", "bar");
}

TEST(c, CHECK_SUBSTREQ)
{
    CHECK_SUBSTREQ("foo", "foobar", strlen("foo"));
}

TEST(c, CHECK_SUBSTRNE)
{
    CHECK_SUBSTRNE("foo", "barfoo", strlen("foo"));
}

TEST(c11, REQUIRE_EQ)
{
    REQUIRE_EQ(1, 1);
}

TEST(c11, REQUIRE_NE)
{
    REQUIRE_NE(1, 2);
}

TEST(c11, REQUIRE_LT)
{
    REQUIRE_LT(1, 2);
}

TEST(c11, REQUIRE_LE) {
    REQUIRE_LE(1, 1);
    REQUIRE_LE(1, 2);
}

TEST(c11, REQUIRE_GT)
{
    REQUIRE_GT(2, 1);
}

TEST(c11, REQUIRE_GE) {
    REQUIRE_GE(1, 1);
    REQUIRE_GE(2, 1);
}

TEST(c11, REQUIRE_STREQ)
{
    REQUIRE_STREQ("foo", "foo");
}

TEST(c11, REQUIRE_SUBSTREQ)
{
    REQUIRE_STRNE("foo", "bar");
}

TEST(c11, CHECK_EQ2)
{
    CHECK_EQ(1, 1);
}

TEST(c11, CHECK_NE)
{
    CHECK_NE(1, 2);
}

TEST(c11, CHECK_LT)
{
    CHECK_LT(1, 2);
}

TEST(c11, CHECK_LE) {
    CHECK_LE(1, 1);
    CHECK_LE(1, 2);
}

TEST(c11, CHECK_GT)
{
    CHECK_GT(3, 1);
}

TEST(c11, CHECK_GE) {
    CHECK_GE(1, 1);
    CHECK_GE(2, 1);
}

TEST(c11, CHECK_STREQ)
{
    CHECK_STREQ("foo", "foo");
}

TEST(c11, CHECK_SUBSTREQ)
{
    CHECK_STRNE("foo", "bar");
}

TEST(c11, no_double_eval) {
    int i = 0;
    REQUIRE_EQ(i++, 0);
    REQUIRE_EQ(i, 1);
}

TEST(c11, CHECK_BUF_EQ) {
	tau_u8 buf[] = {1, 2, 3, 4, 5};
	tau_u8 ref[] = {1, 2, 3, 4, 5};
	CHECK_BUF_EQ(buf, ref, sizeof(ref));
}

TEST(c11, CHECK_BUF_NE) {
	tau_u8 buf[] = {1, 2, 3, 4, 5};
	tau_u8 ref[] = {10, 2, 3, 4, 5};
	CHECK_BUF_NE(buf, ref, sizeof(ref));
}

TEST(c11, REQUIRE_BUF_EQ) {
	tau_u8 buf[] = {1, 2, 3, 4, 5};
	tau_u8 ref[] = {1, 2, 3, 4, 5};
	REQUIRE_BUF_EQ(buf, ref, sizeof(ref));
}

TEST(c11, REQUIRE_BUF_NE) {
	tau_u8 buf[] = {1, 2, 3, 4, 5};
	tau_u8 ref[] = {10, 23, 3, 4, 5};
	REQUIRE_BUF_NE(buf, ref, sizeof(ref));
}

struct MyTestF {
  int foo;
};

TEST_F_SETUP(MyTestF) {
    REQUIRE_EQ(0, tau->foo);
    tau->foo = 42;
}

TEST_F_TEARDOWN(MyTestF)
{
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

TEST(test_tauShouldDecomposeMacro_numbers, decimals)
{
    CHECK_EQ(123456789, 123456789);
    CHECK_EQ(tauShouldDecomposeMacro("1", "123456789", 0), 0);
    CHECK_EQ(tauShouldDecomposeMacro("123456789", "1", 0), 0);
}

TEST(test_tauShouldDecomposeMacro_numbers, decimals_with_sign)
{
    CHECK_EQ(-123456789, -123456789);
    CHECK_EQ(tauShouldDecomposeMacro("-123456789", "1", 0), 0);
    CHECK_EQ(tauShouldDecomposeMacro("1", "-123456789", 0), 0);
    CHECK_EQ(+123456789, +123456789);
    CHECK_EQ(tauShouldDecomposeMacro("+123456789", "1", 0), 0);
    CHECK_EQ(tauShouldDecomposeMacro("1", "+123456789", 0), 0);
}

TEST(test_tauShouldDecomposeMacro_numbers, decimals_with_suffix)
{
    CHECK_EQ(987654321012345678UL, 987654321012345678UL);
    CHECK_EQ(tauShouldDecomposeMacro("987654321012345678UL", "1", 0), 0);
    CHECK_EQ(tauShouldDecomposeMacro("1", "987654321012345678UL", 0), 0);
}

TEST(test_tauShouldDecomposeMacro_numbers, hexadecimals)
{
    CHECK_EQ(0xFFFFFFFF, 0xFFFFFFFF);
    CHECK_EQ(tauShouldDecomposeMacro("0xFFFFFFFF", "1", 0), 0);
    CHECK_EQ(tauShouldDecomposeMacro("1", "0xFFFFFFFF", 0), 0);
}

TEST(test_tauShouldDecomposeMacro_numbers, octals)
{
    CHECK_EQ(007, 007);
    CHECK_EQ(tauShouldDecomposeMacro("1", "007", 0), 0);
    CHECK_EQ(tauShouldDecomposeMacro("007", "1", 0), 0);
}

TEST(test_tauShouldDecomposeMacro_numbers, floats)
{
    CHECK_EQ(420.69, 420.69);
    CHECK_EQ(tauShouldDecomposeMacro("420.69", "1", 0), 0);
    CHECK_EQ(tauShouldDecomposeMacro("1", "420.69", 0), 0);
}

TEST(test_tauShouldDecomposeMacro_numbers, floats_only_fractional_part)
{
    CHECK_EQ(.69, .69);
    CHECK_EQ(tauShouldDecomposeMacro(".69", "1", 0), 0);
    CHECK_EQ(tauShouldDecomposeMacro("1", ".69", 0), 0);
}

TEST(test_tauShouldDecomposeMacro_c_identifiers, letters)
{
    const int myVar = 1;
    CHECK_EQ(myVar, myVar);
    CHECK_EQ(tauShouldDecomposeMacro("myVar", "a", 0), 1);
    CHECK_EQ(tauShouldDecomposeMacro("a", "myVar", 0), 1);
}

TEST(test_tauShouldDecomposeMacro_c_identifiers, letters_and_numbers)
{
    const int v1 = 1;
    CHECK_EQ(v1, v1);
    CHECK_EQ(tauShouldDecomposeMacro("v1", "a", 0), 1);
    CHECK_EQ(tauShouldDecomposeMacro("a", "v1", 0), 1);
}

TEST(test_tauShouldDecomposeMacro_c_identifiers, underscores_and_letters)
{
    const int __myVar__ = 1;
    CHECK_EQ(__myVar__, __myVar__);
    CHECK_EQ(tauShouldDecomposeMacro("__myVar__", "a", 0), 1);
    CHECK_EQ(tauShouldDecomposeMacro("a", "__myVar__", 0), 1);
}

TEST(test_tauShouldDecomposeMacro_c_identifiers, mixed)
{
#define MYMACRO_2024dec10 1
    CHECK_EQ(MYMACRO_2024dec10, MYMACRO_2024dec10);
#undef MYMACRO_2024dec10
    CHECK_EQ(tauShouldDecomposeMacro("MYMACRO_2024dec10", "a", 0), 1);
    CHECK_EQ(tauShouldDecomposeMacro("a", "MYMACRO_2024dec10", 0), 1);
}
