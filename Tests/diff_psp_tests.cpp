#include "gtest/gtest.h"
#include "CF_Grammar.h"
#include "CYK_Manager.hpp"

class DiffPspTest : public ::testing::Test {
 protected:
  void SetUp() {
    CF_Grammar G1("S->(S)S|[S]S|{S}S|;");
    G1.ToHomski();
    CF_Grammar G2("S->(F)F|[F]F|{F}F|;F->(F)F|[F]F|{F}F|;");
    G2.ToHomski();
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

TEST_F(DiffPspTest, BasicTest1) {
  ASSERT_EQ(M1->Recognize("{([])}"), true);
}

TEST_F(DiffPspTest, BasicTest2) {
  ASSERT_EQ(M2->Recognize("{([])}"), true);
}

TEST_F(DiffPspTest, BasicTest3) {
  ASSERT_EQ(M1->Recognize("{([)}"), false);
}

TEST_F(DiffPspTest, BasicTest4) {
  ASSERT_EQ(M2->Recognize("{([)}"), false);
}


TEST_F(DiffPspTest, LittleTest1) {
  ASSERT_EQ(M1->Recognize("{}[]{{}}()"), true);
}

TEST_F(DiffPspTest, LittleTest2) {
  ASSERT_EQ(M2->Recognize("{}[]{{}}()"), true);
}

TEST_F(DiffPspTest, LittleTest3) {
  ASSERT_EQ(M1->Recognize("{}[]{}{()]"), false);
}

TEST_F(DiffPspTest, LittleTest4) {
  ASSERT_EQ(M2->Recognize("{}[]{}{()]"), false);
}

TEST_F(DiffPspTest, LittleTest5) {
  ASSERT_EQ(M1->Recognize("{}[{{}()}]"), true);
}

TEST_F(DiffPspTest, LittleTest6) {
  ASSERT_EQ(M2->Recognize("{}[{{}()}]"), true);
}

TEST_F(DiffPspTest, LittleTest7) {
  ASSERT_EQ(M1->Recognize("{}[{}()}"), false);
}

TEST_F(DiffPspTest, LittleTest8) {
  ASSERT_EQ(M2->Recognize("{}[{}()}"), false);
}

TEST_F(DiffPspTest, LittleTest9) {
  ASSERT_EQ(M1->Recognize("{}[]{{}}()"), true);
}

TEST_F(DiffPspTest, LittleTest10) {
  ASSERT_EQ(M1->Recognize("{}[]{{}}()"), true);
}

TEST_F(DiffPspTest, MediumTest1) {
  ASSERT_EQ(M2->Recognize("[()][()]{}[[((({}{})))]]{}[()][()]"), true);
}

TEST_F(DiffPspTest, MediumTest2) {
  ASSERT_EQ(M1->Recognize("[()][()]{}[[((({}{})))]]{}[()][()]"), true);
}

TEST_F(DiffPspTest, MediumTest3) {
  ASSERT_EQ(M2->Recognize("[()][)]{}[[(({}{}))]]{}[)][()]"), false);
}

TEST_F(DiffPspTest, MediumTest4) {
  ASSERT_EQ(M1->Recognize("[()][()]{[[(((}{}))]]{}()][()]"), false);
}

TEST_F(DiffPspTest, LongTest1) {
  ASSERT_EQ(M1->Recognize("(([{}[(({}))]{}](())[[]]([{{}[[]]{}}])))"), true);
}

TEST_F(DiffPspTest, LongTest2) {
  ASSERT_EQ(M2->Recognize("(([{}[(({}))]{}](())[[]]([{{}[[]]{}}])))"), true);
}


TEST_F(DiffPspTest, LongTest3) {
  ASSERT_EQ(M1->Recognize("(([{}(({}))]{}]())[[]]([{}[[]]{}})))"), false);
}

TEST_F(DiffPspTest, LongTest4) {
  ASSERT_EQ(M2->Recognize("(([{[(({}))]{}(())[[]]([{}[[]]{}})))"), false);
}

TEST_F(DiffPspTest, LongTest5) {
  ASSERT_EQ(M1->Recognize("(())[][]{{{}}}{{(([[][]]))}}{{(())}}{}{}()()()()[][]"), true);
}

TEST_F(DiffPspTest, LongTest6) {
  ASSERT_EQ(M2->Recognize("(())[][]{{{}}}{{(([[][]]))}}{{(())}}{}{}()()()()[][]"), true);
}

TEST_F(DiffPspTest, LongTest7) {
  ASSERT_EQ(M1->Recognize("(())][]{{{}}}{{(([[][]))}}{{(())}}{}{}()()()()[][]"), false);
}

TEST_F(DiffPspTest, LongTest8) {
  ASSERT_EQ(M2->Recognize("(())[]]{{{}}}{{(([[[]]))}}{{(())}}{}{}()()()()[][]"), false);
}
