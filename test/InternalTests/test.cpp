#include <tau/tau.h>
// Only MSVC seems to complain about this
// Most likely because we're trying to cross-compile with `main.c` and `test.cpp`
#ifdef _MSC_VER
    TAU_ONLY_GLOBALS()
#endif // _MSC_VER

TEST(cpp, REQUIRE_EQ) { 
    REQUIRE_EQ(1, 1); 
}

TEST(cpp, REQUIRE_NE) { 
    REQUIRE_NE(1, 2); 
}

TEST(cpp, REQUIRE_LT) { 
    REQUIRE_LT(1, 2); 
}

TEST(cpp, REQUIRE_LE) {
  REQUIRE_LE(1, 1);
  REQUIRE_LE(1, 2);
}

TEST(cpp, REQUIRE_GT) { 
    REQUIRE_GT(2, 1); 
}

TEST(cpp, REQUIRE_GE) {
  REQUIRE_GE(1, 1);
  REQUIRE_GE(2, 1);
}

TEST(cpp, REQUIRE_STREQ) { 
    REQUIRE_STREQ("foo", "foo"); 
}

TEST(cpp, REQUIRE_STRNE) { 
    REQUIRE_STRNE("foo", "bar"); 
}

TEST(cpp, REQUIRE_SUBSTREQ) { 
    REQUIRE_SUBSTREQ("foo", "foo", strlen("foo")); 
}

TEST(cpp, REQUIRE_SUBSTRNE) { 
    REQUIRE_SUBSTRNE("foo", "barfoo", strlen("foo")); 
}


TEST(cpp, CHECK_EQ) { 
    CHECK_EQ(1, 1); 
}

TEST(cpp, CHECK_NE) { 
    CHECK_NE(1, 2); 
}

TEST(cpp, CHECK_LT) { 
    CHECK_LT(1, 2); 
}

TEST(cpp, CHECK_LE) {
  CHECK_LE(1, 1);
  CHECK_LE(1, 2);
}

TEST(cpp, CHECK_GT) { CHECK_GT(2, 1); }

TEST(cpp, CHECK_GE) {
  CHECK_GE(1, 1);
  CHECK_GE(2, 1);
}

// TEST(cpp, CHECK_STREQ) { 
//     CHECK_STREQ("foo", "foo"); 
// }

// TEST(cpp, CHECK_STRNE) { 
//     CHECK_STRNE("foo", "bar"); 
// }

// TEST(cpp, CHECK_STRMNEQ) { 
//     CHECK_STRNNEQ("foo", "foobar", strlen("food")); 
// }

// TEST(cpp, CHECK_STRNNEQ) { 
//     CHECK_STRNNEQ("foo", "barfoo", strlen("foo")); 
// }

TEST(cpp, no_double_eval) {
  int i = 0;
  REQUIRE_EQ(i++, 0);
  REQUIRE_EQ(i, 1);
}


TEST(cpp, Float) {
    float a = 1;
    float b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, Double) {
    double a = 1;
    double b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, LongDouble) {
    long double a = 1;
    long double b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, Char) {
    signed char a = 1;
    signed char b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, UChar) {
    unsigned char a = 1;
    unsigned char b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, Short) {
    short a = 1;
    short b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, UShort) {
    unsigned short a = 1;
    unsigned short b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, Int) {
    int a = 1;
    int b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, UInt) {
    unsigned int a = 1;
    unsigned int b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, Long) {
    long a = 1;
    long b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, ULong) {
    unsigned long a = 1;
    unsigned long b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp, Ptr) {
    char foo = 42;
    CHECK_NE(&foo, &foo + 1);
}

static const int cppdata[4] = {42, 13, 6, -53};

TEST(cpp, Array) { 
    CHECK_NE(cppdata, cppdata + 1); 
}

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

TEST(cpp11, REQUIRE_STRNE) { 
    REQUIRE_STRNE("foo", "bar"); 
}

TEST(cpp11, REQUIRE_SUBSTREQ) { 
    REQUIRE_SUBSTREQ("foo", "foo", strlen("foo")); 
}

TEST(cpp11, REQUIRE_SUBSTRNE) { 
    REQUIRE_SUBSTRNE("foo", "barfoo", strlen("foo")); 
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

TEST(cpp11, CHECK_STRNE) { 
    CHECK_STRNE("foo", "bar"); 
}

TEST(cpp11, CHECK_SUBSTREQ) { 
    CHECK_SUBSTREQ("foo", "foo", strlen("foo")); 
}

TEST(cpp11, CHECK_SUBSTRNE) { 
    CHECK_SUBSTRNE("foo", "barfoo", strlen("foo")); 
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

TEST(cpp11, WOOOOOOOOOOOOOOOO) {
    unsigned long a = 1;
    unsigned long b = 2;
    CHECK_NE(a, b);
    REQUIRE_NE(a, b);
}

TEST(cpp11, Ptr) {
    char foo = 42;
    CHECK_NE(&foo, &foo + 1);
}

static const int cpp11[4] = {42, 13, 6, -53};

TEST(cpp11, Array) { 
    CHECK_NE(cpp11, cpp11 + 1); 
}

TEST(cpp, Section) {
    SECTION("#1") {
        CHECK_NE(1, 2);
    }
}

struct Stack {
    char* name; 
    int age;

    int pop() {
        return 123;
    }

    void test() {
        CHECK_EQ(12, 12);
    }
};

TEST_F_SETUP(Stack) {
    WARN(TESTING WARNING);
    tau->name = "Hello";
    tau->age = 4;
}

TEST_F_TEARDOWN(Stack) {

}

TEST_F(Stack, TestStackDetails) {
    tau->test();
    CHECK_EQ(tau->age, 4);
    REQUIRE_STREQ(tau->name, "Hello");
    REQUIRE_EQ(tau->pop(), 123);
}