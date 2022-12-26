#include "gtest/gtest.h"
#include "CF_Grammar.h"
#include "CYK_Manager.hpp"

class CW_Tester : public ::testing::Test {
 protected:
  void SetUp() {
    CF_Grammar G("S->A|AA|AAA;A->bAA|cAA|aC;C->aB|bA|cA|;B->bC|cC|aBB;"); // |w|_a - |w|_b - |w|_c \in {1, 2, 3}
    G.ToHomski();
    M = new CYK_Manager(G);
  }
  void TearDown() {
    delete M;
  }
  CYK_Manager* M;
};

TEST_F(CW_Tester, basic1) {
  ASSERT_EQ(M->Recognize("a"), true);
}
TEST_F(CW_Tester, basic2) {
  ASSERT_EQ(M->Recognize("b"), false);
}
TEST_F(CW_Tester, basic3) {
  ASSERT_EQ(M->Recognize("bcaa"), false);
}
TEST_F(CW_Tester, basic4) {
  ASSERT_EQ(M->Recognize("abaca"), true);
}
TEST_F(CW_Tester, basic5) {
  ASSERT_EQ(M->Recognize("aaaba"), true);
}

TEST_F(CW_Tester, medium_test1) {
  ASSERT_EQ(M->Recognize("bbaaabaabababcabaa"), true);
}

TEST_F(CW_Tester, medium_test2) {
  ASSERT_EQ(M->Recognize("aaaaaaaaaabbbbbcccccaa"), true);
}

TEST_F(CW_Tester, medium_test3) {
  ASSERT_EQ(M->Recognize("aaaaaaaaaaaaabbbbbcccccaa"), false);
}

TEST_F(CW_Tester, long_test1) {
  ASSERT_EQ(M->Recognize("abcabacabacabacabacabacabacabacabacabacabacaca"), false);
}

TEST_F(CW_Tester, long_test2) {
  ASSERT_EQ(M->Recognize("aaaababababababbabaababbabaababbabaababbaba"), true);
}

TEST_F(CW_Tester, long_test3) {
  ASSERT_EQ(M->Recognize("aaaabababababababababbabaabcacacaabbabaababbabaababbaaaabbbbaaaabbbbaaaabbbbaaaabbbbaba"), true);
}

TEST_F(CW_Tester, long_test4) {
  ASSERT_EQ(M->Recognize("aaaabababababababababbabaabcacacaabbabaababbabaababbaaaabbbbaaaabbbbaaaabbbbaaaabbbbabaa"), false);
}

TEST_F(CW_Tester, long_test5) {
  ASSERT_EQ(M->Recognize("aaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbba"), true);
}

TEST_F(CW_Tester, long_test6) {
  ASSERT_EQ(M->Recognize("aaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbb"), false);
}