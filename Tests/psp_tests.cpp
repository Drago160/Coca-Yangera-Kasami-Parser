#include "gtest/gtest.h"
#include "CYK_Manager.hpp"
#include "CF_Grammar.h"

class PspTest : public ::testing::Test {
 protected:
  void SetUp() {
    ChomskyGrammar G1("S->(S)S|;");
    ChomskyGrammar G2("S->(F)F|;F->(F)F|;");
    M1 = new CYK_Manager(G1);
    M2 = new CYK_Manager(G2);
  }
  void TearDown() {
    delete M1;
    delete M2;
  }
  CYK_Manager* M1;
  CYK_Manager* M2;
};
TEST_F(PspTest, BasicTest1) {
  ASSERT_EQ(M1->Recognize("()"), true);
}

TEST_F(PspTest, BasicTest2) {
  ASSERT_EQ(M2->Recognize("()"), true);
}


TEST_F(PspTest, TestEmpty1) {
  ASSERT_EQ(M2->Recognize(""), true);
}

TEST_F(PspTest, TestEmpty2) {
  ASSERT_EQ(M2->Recognize(""), true);
}

TEST_F(PspTest, MediumTest1) {
  ASSERT_EQ(M1->Recognize("(()())()()((())())"), true);
}

TEST_F(PspTest, MediumTest2) {
  ASSERT_EQ(M2->Recognize("(()())()()((())())"), true);
}
TEST_F(PspTest, LongTest1) {
  ASSERT_EQ(M1->Recognize("()()()(())()()()()((()))()()()()((()))((()))(())()()"), true);
}

TEST_F(PspTest, LongTest2) {
  ASSERT_EQ(M2->Recognize("()()()(())()()()()((()))()()()()((()))((()))(())()()"), true);
}
TEST_F(PspTest, RandomStressTests) {
  //  Generate psp with size near 1000
  const size_t iter_nums = 30;
  const size_t min_size = 40;
  std::string str;
  for (size_t i = 0; i < iter_nums; ++i) {
    size_t breacket_res = 0;
    str = "";
    while (str.size() < min_size) {
      if (breacket_res == 0) {
        str.push_back('(');
        ++breacket_res;
      }
      if (rand() % 2 == 0) {
        str.push_back('(');
        ++breacket_res;
      } else {
        str.push_back(')');
        --breacket_res;
      }
    }
    while (breacket_res != 0) {
      str.push_back(')');
      breacket_res--;
    }

    ASSERT_EQ(M1->Recognize(str), true);
    ASSERT_EQ(M2->Recognize(str), true);
  }

}

TEST_F(PspTest, OneBreacketTest1) {
  ASSERT_EQ(M1->Recognize("("), false);
}

TEST_F(PspTest, OneBreacketTest2) {
  ASSERT_EQ(M2->Recognize("("), false);
}

TEST_F(PspTest, MediumTest3) {
  ASSERT_EQ(M1->Recognize("())((()((())))((()()("), false);
}

TEST_F(PspTest, MediumTest4) {
  ASSERT_EQ(M1->Recognize("())((()((())))((()()("), false);
}

TEST_F(PspTest, MediumTest5) {
  ASSERT_EQ(M1->Recognize( "()(()))((()()(()"), false);
}

TEST_F(PspTest, MediumTest6) {
  ASSERT_EQ(M1->Recognize("()(()))((()()(()"), false);
}

TEST_F(PspTest, StartTest1) {
  ASSERT_EQ(M1->Recognize( ")(()()()"), false);
}

TEST_F(PspTest, StartTest2) {
  ASSERT_EQ(M1->Recognize(")(()()()"), false);
}

TEST_F(PspTest, MediumTest7) {
  ASSERT_EQ(M1->Recognize( "()(())(()))(()()"), false);
}

TEST_F(PspTest, MediumTest8) {
  ASSERT_EQ(M1->Recognize("()(())(()))(()()"), false);
}
