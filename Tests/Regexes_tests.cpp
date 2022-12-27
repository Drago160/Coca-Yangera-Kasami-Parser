#include "gtest/gtest.h"
#include "CF_Grammar.h"
#include "CYK_Manager.hpp"

class Regex1 : public ::testing::Test {
 protected:
  void SetUp() {
    ChomskyGrammar G1("S->aA|Aa|A;A->aA|a;");
    M1 = new CYK_Manager(G1);
  }
  void TearDown() {
    delete M1;
  }
  CYK_Manager* M1; // a+
};

class Regex2 : public ::testing::Test {
 protected:
  void SetUp() {
    ChomskyGrammar G2("S->aB|b;B->bB|bS|bb;");
    M2 = new CYK_Manager(G2);
  }
  void TearDown() {
    delete M2;
  }
  CYK_Manager* M2; // (ab+)*b
};


class Regex3 : public ::testing::Test {
 protected:
  void SetUp() {
    ChomskyGrammar G3("S->aS|bS|B;B->bbC;C->aC|bC|aa|ba;");
    M3 = new CYK_Manager(G3);
  }
  void TearDown() {
    delete M3;
  }
  CYK_Manager* M3; // (a+b)*bb(a+b)+a
};


TEST_F(Regex1, Test1) {
  ASSERT_EQ(M1->Recognize("a"), true);
}

TEST_F(Regex1, Test2) {
  ASSERT_EQ(M1->Recognize("aaaa"), true);
}
TEST_F(Regex1, Test3) {
  ASSERT_EQ(M1->Recognize("aaaaaaaaaaaa"), true);
}

TEST_F(Regex1, Test4) {
  ASSERT_EQ(M1->Recognize("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), true);
}

TEST_F(Regex1, Test5) {
  ASSERT_EQ(M1->Recognize("b"), false);
}

TEST_F(Regex1, Test6) {
  ASSERT_EQ(M1->Recognize("aaaab"), false);
}

TEST_F(Regex1, Test7) {
  ASSERT_EQ(M1->Recognize("aaaaaaaaaaaaaaaaaab"), false);
}
TEST_F(Regex1, Test8) {
  EXPECT_EQ(M1->Recognize("aaaaaaaaaaaaaaaaaabaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), false);
}

TEST_F(Regex2, Test1) {
  ASSERT_EQ(M2->Recognize("abb"), true);
}

TEST_F(Regex2, Test2) {
  ASSERT_EQ(M2->Recognize("b"), true);
}

TEST_F(Regex2, Test3) {
  ASSERT_EQ(M2->Recognize("abbabbabb"), true);
}

TEST_F(Regex2, Test4) {
  ASSERT_EQ(M2->Recognize("abbbbbbbbabbbbbbbbbabbbbbbbbbbbabbbbbbb"), true);
}

TEST_F(Regex2, Test5) {
  ASSERT_EQ(M2->Recognize("abababababababbbbbbbbbabbbbbbbababababababababbbbbbbbabbbbbbbbbabbbbbbbbbbbabbbbbbb"), true);
}

TEST_F(Regex2, Test6) {
  ASSERT_EQ(M2->Recognize("aabababababbbaaaaaa"), false);
}

TEST_F(Regex2, Test7) {
  ASSERT_EQ(M2->Recognize("a"), false);
}

TEST_F(Regex2, Test8) {
  ASSERT_EQ(M2->Recognize(""), false);
}

TEST_F(Regex2, Test9) {
  ASSERT_EQ(M2->Recognize("bbbaaaaaaaaa"), false);
}

// (a+b)*bb(a+b)+a
TEST_F(Regex3, Test1) {
  ASSERT_EQ(M3->Recognize("bbaa"), true);
}

TEST_F(Regex3, Test2) {
  ASSERT_EQ(M3->Recognize("abaabababbba"), true);
}

TEST_F(Regex3, Test3) {
  ASSERT_EQ(M3->Recognize("aaaaaaabbaa"), true);
}

TEST_F(Regex3, Test4) {
  ASSERT_EQ(M3->Recognize("aabababababbabbabababaaaabbabababbabababbabbbaa"), true);
}

TEST_F(Regex3, Test5) {
  ASSERT_EQ(M3->Recognize("bbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), true);
}

TEST_F(Regex3, Test6) {
  ASSERT_EQ(M3->Recognize("aaabaa"), false);
}

TEST_F(Regex3, Test7) {
  ASSERT_EQ(M3->Recognize("aaabba"), false);
}

TEST_F(Regex3, Test8) {
  ASSERT_EQ(M3->Recognize("aaabaa"), false);
}

TEST_F(Regex3, Test9) {
  ASSERT_EQ(M3->Recognize("bbbbbbbbbbbbbbbb"), false);
}