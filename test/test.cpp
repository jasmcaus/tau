#include <Muon/Muon.h>

TEST(cpp11, REQUIRE_EQ) { 
    REQUIRE_EQ(1, 1); 
}

TEST(cpp11, REQUIRE_NE) { 
    REQUIRE_NE(1, 2); 
}

TEST(cpp11, REQUIRE_LT) { 
    REQUIRE_LT(1, 2); 
}

TEST(cpp11, REQUIRE_LE) {
  REQUIRE_LE(1, 1);
  REQUIRE_LE(1, 2);
}

TEST(cpp11, REQUIRE_GT) { REQUIRE_GT(2, 1); }

TEST(cpp11, REQUIRE_GE) {
  REQUIRE_GE(1, 1);
  REQUIRE_GE(2, 1);
}

TEST(cpp11, REQUIRE_STREQ) { 
    REQUIRE_STREQ("foo", "foo"); 
}

TEST(cpp11, REQUIRE_STRNEQ) { 
    REQUIRE_STRNEQ("foo", "bar"); 
}

TEST(cpp11, REQUIRE_STRNNEQ) { 
    REQUIRE_STRNNEQ("foo", "foobar", strlen("foo")); 
}

TEST(cpp11, CHECK_EQ) { 
    CHECK_EQ(1, 1); 
}

TEST(cpp11, CHECK_NE) { 
    CHECK_NE(1, 2); 
}

TEST(cpp11, CHECK_LT) { 
    CHECK_LT(1, 2); 
}

TEST(cpp11, CHECK_LE) {
  CHECK_LE(1, 1);
  CHECK_LE(1, 2);
}

TEST(cpp11, CHECK_GT) { 
    CHECK_GT(2, 1); 
}

TEST(cpp11, CHECK_GE) {
  CHECK_GE(1, 1);
  CHECK_GE(2, 1);
}

TEST(cpp11, CHECK_STREQ) { 
    CHECK_STREQ("foo", "foo"); 
}

TEST(cpp11, CHECK_STRNEQ) { 
    CHECK_STRNEQ("foo", "bar"); 
}

TEST(cpp11, CHECK_STRNNEQ) { 
    CHECK_STRNNEQ("foo", "barfoo", strlen("foo")); 
}

TEST(cpp11, no_double_eval) {
    int i = 0;
    REQUIRE_EQ(i++, 0);
    REQUIRE_EQ(i, 1);
}

TEST(cpp11, Float) {
    float a = 1;
    float b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, Double) {
    double a = 1;
    double b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, LongDouble) {
    long double a = 1;
    long double b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, Char) {
    signed char a = 1;
    signed char b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, UChar) {
    unsigned char a = 1;
    unsigned char b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, Short) {
    short a = 1;
    short b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, UShort) {
    unsigned short a = 1;
    unsigned short b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, Int) {
    int a = 1;
    int b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, UInt) {
    unsigned int a = 1;
    unsigned int b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, Long) {
    long a = 1;
    long b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, ULong) {
    unsigned long a = 1;
    unsigned long b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, Ptr) {
    char foo = 42;
    CHECK_NE(&foo, &foo + 1);
}

TEST(cpp11, VoidPtr) {
    void *foo = reinterpret_cast<void*>(0);
    CHECK_NE(foo, static_cast<char*>(foo) + 1);
}

static const int data[4] = {42, 13, 6, -53};

TEST(cpp11, Array) { 
    CHECK_NE(data, data + 1); 
}