#include "gtest/gtest.h"
#include "CF_Grammar.h"
#include "CYK_Manager.hpp"

class BalancedTest : public ::testing::Test {
 protected:
  void SetUp() {
    CF_Grammar G("S->aA|bB|;A->aAA|bS|a;B->bBB|aS|b;");
    G.ToHomski();
    M = new CYK_Manager(G);
  }
   void TearDown() {
    delete M;
  }
  CYK_Manager* M;
};
TEST_F(BalancedTest, BasicTest1) {
  ASSERT_EQ(M->Recognize(""), true);
}

TEST_F(BalancedTest, BasicTest2) {
  ASSERT_EQ(M->Recognize("ab"), true);
}

TEST_F(BalancedTest, twoBlocks) {
  ASSERT_EQ(M->Recognize("bbbbaaaa"), true);
}

TEST_F(BalancedTest, someBlocks) {
  ASSERT_EQ(M->Recognize("bbbbaaaa"), true);
}

TEST_F(BalancedTest, LongTest) {
  ASSERT_EQ(M->Recognize("abbaabbabaabbaabbaabbaabbaabbaabbaabbaabbaababababababbbbaaaababbababa"), true);
}

TEST_F(BalancedTest, RandomTests) {
  int balance = 0;
  const size_t num_of_tests = 10;
  std::string s;
  const size_t max_size = 50;
  for (size_t k = 0; k < num_of_tests; ++k) {
    s = "";
    balance = 0;
    while (s.size() < max_size) {
      if (rand() % 2 == 1) {
        s.push_back('a');
        ++balance;
      } else {
        s.push_back('b');
        --balance;
      }
    }
    while (balance > 0) {
      s.push_back('b');
      --balance;
    }
    while (balance < 0) {
      s.push_back('a');
      ++balance;
    }
    ASSERT_EQ(M->Recognize(s), true);
  }
}

TEST_F(BalancedTest, BasicTest3) {
  ASSERT_EQ(M->Recognize("b"), false);
}

TEST_F(BalancedTest, BasicTest4) {
  ASSERT_EQ(M->Recognize("bab"), false);
}

TEST_F(BalancedTest, twoBlock2) {
  ASSERT_EQ(M->Recognize("bbbbaaaaa"), false);
}

TEST_F(BalancedTest, twoBlock3) {
  ASSERT_EQ(M->Recognize("bbbbbaaaa"), false);
}

TEST_F(BalancedTest, LongTest2) {
  ASSERT_EQ(M->Recognize("bbbbbaaabababababbababababaaabababbabbbabababababbaaaaaaaababbabbabbabababbabaa"), false);
}

TEST_F(BalancedTest, LongTest3) {
  ASSERT_EQ(M->Recognize("bbbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaababababbaaaaaaaababbabbabbabababbabaa"), false);
}
