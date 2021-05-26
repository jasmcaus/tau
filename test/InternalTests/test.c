#include "Muon/Muon.h"

// TEST(c, MemAlloc) {
//     void* mem;
    
//     mem = malloc(5);
//     CHECK(mem != NULL, "Mem ideally should not be NULL");

//     mem = realloc(mem, 10);
//     CHECK(mem != NULL, "Mem ideally should not be NULL");
// }

// TEST(c, ForLoop) { 
//     int count = 0;
//     for(int i = 0; i<10000000; i++) {
//         count ++;
//     }
//     REQUIRE_EQ(count, 10000000); 
// }

// TEST(c, CHECK) { 
//     CHECK(1 < 2, "1 < 2"); 
// }

// TEST(c, REQUIRE_LT) { 
//     REQUIRE_LT(1, 2);  
// }

// TEST(c, REQUIRE_LE) {
//     REQUIRE_LE(1, 1);
//     REQUIRE_LE(1, 2);
// }

// TEST(c, REQUIRE_GT) { 
//     REQUIRE_GT(2, 1); 
// }

// TEST(c, REQUIRE_GE) {
//     REQUIRE_GE(1, 1);
//     REQUIRE_GE(2, 1);
// }

// TEST(c, REQUIRE_STREQ) { 
//     REQUIRE_STREQ("foo", "foo"); 
// }

// TEST(c, REQUIRE_STRNE) { 
//     REQUIRE_STRNEQ("foo", "bar"); 
// }

// TEST(c, REQUIRE_STRNEQ) { 
//     REQUIRE_STRNEQ("foo", "foobar"); 
// }

// TEST(c, REQUIRE_STRNNE) { 
//     REQUIRE_STRNNEQ("foo", "barfoo", strlen("foo")); 
// }

// TEST(c, CHECK_EQ1) { 
//     CHECK_EQ(345, 345); 
// }

// TEST(c, CHECK_NE) { 
//     CHECK_NE(1, 2); 
// }

// TEST(c, CHECK_LT) { 
//     CHECK_LT(1, 2); 
// }

// TEST(c, CHECK_LE) {
//     CHECK_LE(1, 1);
//     CHECK_LE(1, 2);
// }

// TEST(c, CHECK_GT) { CHECK_GT(2, 1); }

// TEST(c, CHECK_GE) {
//     CHECK_GE(1, 1);
//     CHECK_GE(2, 1);
// }

// TEST(c, CHECK_STREQ) { 
//     CHECK_STREQ("foo", "foo"); 
// }

// TEST(c, CHECK_STRNE) { 
//     CHECK_STRNEQ("foo", "food"); 
// }

// TEST(c, CHECK_STRNNE) { 
//     CHECK_STRNNEQ("foo", "barfoo", strlen("foo")); 
// }

// TEST(c11, REQUIRE_EQ) { 
//     REQUIRE_EQ(1, 1); 
// }

// TEST(c11, REQUIRE_NE) { 
//     REQUIRE_NE(1, 2); 
// }

// TEST(c11, REQUIRE_LT) { 
//     REQUIRE_LT(1, 2); 
// }

// TEST(c11, REQUIRE_LE) {
//     REQUIRE_LE(1, 1);
//     REQUIRE_LE(1, 2);
// }

// TEST(c11, REQUIRE_GT) { 
//     REQUIRE_GT(2, 1); 
// }

// TEST(c11, REQUIRE_GE) {
//     REQUIRE_GE(1, 1);
//     REQUIRE_GE(2, 1);
// }

// TEST(c11, REQUIRE_STREQ) { 
//     REQUIRE_STREQ("foo", "foo"); 
// }

// TEST(c11, REQUIRE_STRNE) { 
//     REQUIRE_STRNEQ("foo", "bar"); 
// }

// TEST(c11, CHECK_EQ2) { 
//     CHECK_EQ(1, 1); 
// }

// TEST(c11, CHECK_NE) { 
//     CHECK_NE(1, 2); 
// }

// TEST(c11, CHECK_LT) { 
//     CHECK_LT(1, 2); 
// }

// TEST(c11, CHECK_LE) {
//     CHECK_LE(1, 1);
//     CHECK_LE(1, 2);
// }

TEST(c11, CHECK_GT) { 
    CHECK_GT(0, 1); 
}

// TEST(c11, CHECK_GE) {
//     CHECK_GE(1, 1);
//     CHECK_GE(2, 1);
// }

// TEST(c11, CHECK_STREQ) { 
//     CHECK_STREQ("foo", "foo"); 
// }

// TEST(c11, CHECK_STRNE) { 
//     CHECK_STRNEQ("foo", "bar"); 
// }

// TEST(c11, no_double_eval) {
//     int i = 0;
//     REQUIRE_EQ(i++, 0);
//     REQUIRE_EQ(i, 1);
// }

// struct MyTestF {
//   int foo;
// };

// TEST_F_SETUP(MyTestF) {
//     REQUIRE_EQ(0, muon->foo);
//     muon->foo = 42;
// }

// TEST_F_TEARDOWN(MyTestF) { 
//     REQUIRE_EQ(13, muon->foo); 
// }

// TEST_F(MyTestF, c) {
//     REQUIRE_EQ(42, muon->foo);
//     muon->foo = 13;
// }

// TEST_F(MyTestF, c2) {
//     REQUIRE_EQ(42, muon->foo);
//     muon->foo = 13;
// }