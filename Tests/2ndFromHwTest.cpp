#include "gtest/gtest.h"
#include "CF_Grammar.h"
#include "CYK_Manager.hpp"

class HWTester : public ::testing::Test {
 protected:
  void SetUp() {
    CF_Grammar G("S->aA|bB;A->aC|bS;B->aS|bK;K->aB;C->bA|aR|;R->aRR|bC;");
    G.ToHomski();
    M = new CYK_Manager(G);
    /*G = new CF_Grammar({'S', 'A', 'B', 'C', 'K', 'R'},
                       {'a', 'b'},

                       {{"S", "aA", "bB"},
                        {"A", "aC", "bS"},
                        {"B", "aS", "bK"},
                        {"K", "aB"},
                        {"C", "bA", "aR", ""},
                        {"R", "aRR", "bC"}},

                        'S');
                        */
  }
  CYK_Manager* M;
};

TEST_F(HWTester, BasicTest) {
  ASSERT_EQ(M->Recognize("aa"), true);
}
TEST_F(HWTester, BasicTest2) {
  ASSERT_EQ(M->Recognize("bbaaaa"), true);
}

TEST_F(HWTester, BasicTest3) {
  ASSERT_EQ(M->Recognize("baabaa"), true);
}

TEST_F(HWTester, BasicTest4) {
  ASSERT_EQ(M->Recognize("abaaab"), true);
}

TEST_F(HWTester, BigDeviation1) {
  ASSERT_EQ(M->Recognize("aaaaaaaaaaaaaaaabbbbbbbbbbbbbb"), true);
}

TEST_F(HWTester, Fluctuation1) {
  ASSERT_EQ(M->Recognize("bbaabbaabbaaaabbaaaaaabbbb"), true);
}

TEST_F(HWTester, Fluctuation2) {
  ASSERT_EQ(M->Recognize("ababababababababababababababababaa"), true);
}

TEST_F(HWTester, LowPref1) {
  ASSERT_EQ(M->Recognize("bbbaaaaa"), false);
}

TEST_F(HWTester, LowPref2) {
  ASSERT_EQ(M->Recognize("bbabaaaaa"), false);
}

TEST_F(HWTester, LowPref3) {
  ASSERT_EQ(M->Recognize("aaaabbbbabbabbabbaaaaaa"), false);
}

TEST_F(HWTester, WrongModule1) {
  ASSERT_EQ(M->Recognize("aababababaabaabababababababaaaabaabab"), false);
}

TEST_F(HWTester, WrongModule2) {
  ASSERT_EQ(M->Recognize("aababababaabaababababaabaababababaababbababbababbaaaabaaababbabab"), false);
}