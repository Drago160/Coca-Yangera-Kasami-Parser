#include "gtest/gtest.h"
#include "CF_Grammar.h"
#include "Builder.hpp"

class ChomskyGrammar::BuildTester {
 public:
  static bool NonGenerativeBasicTest() {
    ChomskyGrammar grammar;
    grammar.rules_ = CF_Grammar("S->a|A;A->Aa;").GetRules();
    ChomskyGrammar::Builder builder(grammar);
    builder.DeleteNonGenerative();
    CF_Grammar g("S->a;");
    return (grammar.rules_ == CF_Grammar("S->a;").GetRules());
  }

  static bool NonGenerativeTest() {
    ChomskyGrammar grammar;
    grammar.rules_ = CF_Grammar("S->a|B|A;B->A|BB|b;A->aA|A;").GetRules();
    ChomskyGrammar::Builder builder(grammar);
    builder.DeleteNonGenerative();
    CF_Grammar g("S->a|B;B->BB|b;");
    return (grammar.rules_ == g.GetRules());
  }

  static bool UnReachableBasicTest() {
    ChomskyGrammar grammar;
    grammar.rules_ = CF_Grammar("S->A|;A->a;B->b;").GetRules();
    ChomskyGrammar::Builder builder(grammar);
    builder.DeleteUnReachable();
    CF_Grammar g("S->A|;A->a;");
    return (grammar.rules_ == g.GetRules());
  }

  static bool UnReachableTest() {
    ChomskyGrammar grammar;
    grammar.rules_ = CF_Grammar("S->A|;A->a;B->b|A|E;C->S|;D->E;E->SAS|a;B->E;").GetRules();
    ChomskyGrammar::Builder builder(grammar);
    builder.DeleteUnReachable();
    CF_Grammar g("S->A|;A->a;");
    return (grammar.rules_ == g.GetRules());
  }

  static bool ExistMixed(const auto& rules) {
    for (const auto& [src, r_parts] : rules) {
      for (const auto& dst : r_parts) {
        if (ChomskyGrammar::Builder::IsMixed(dst)) {
          return false;
        }
      }
    }
    return true;
  }

  static bool CheckMixedBasic() {
    ChomskyGrammar grammar;
    grammar.terminals_ = {-1};
    grammar.non_terminals_ = {2, 3};
    grammar.rules_[2] = {{3, -1, 3, -1}};
    grammar.max_nonterm_ = 3;
    grammar.min_term_ = -1;
    ChomskyGrammar::Builder builder(grammar);

    builder.HandleMixed();

    ChomskyGrammar expected_grammar;
    expected_grammar.terminals_ = {-1};
    expected_grammar.non_terminals_ = {2, 3, 4};
    expected_grammar.rules_[2] = {{3, 4, 3, 4}};
    expected_grammar.rules_[4] = {{-1}};
    expected_grammar.max_nonterm_ = 4;
    expected_grammar.min_term_ = -1;

    return (expected_grammar.rules_ == grammar.rules_ &&
            expected_grammar.terminals_ == grammar.terminals_ &&
            expected_grammar.non_terminals_ == grammar.non_terminals_ &&
            expected_grammar.max_nonterm_ == grammar.max_nonterm_);
  }

  static bool CheckMixed() {
    ChomskyGrammar grammar;
    grammar.terminals_ = {-1};
    grammar.non_terminals_ = {2, 3};
    grammar.rules_[2] = {{3, -1}};
    grammar.rules_[3] = {{3, -1}, {3}};
    grammar.max_nonterm_ = 3;
    grammar.min_term_ = -1;
    ChomskyGrammar::Builder builder(grammar);

    builder.HandleMixed();

    ChomskyGrammar expected_grammar;
    expected_grammar.terminals_ = {-1};
    expected_grammar.non_terminals_ = {2, 3, 4};
    expected_grammar.rules_[2] = {{3, 4}};
    expected_grammar.rules_[3] = {{3, 4}, {3}};
    expected_grammar.rules_[4] = {{-1}};
    expected_grammar.max_nonterm_ = 4;
    expected_grammar.min_term_ = -1;

    return (expected_grammar.rules_ == grammar.rules_ &&
        expected_grammar.terminals_ == grammar.terminals_ &&
        expected_grammar.non_terminals_ == grammar.non_terminals_ &&
        expected_grammar.max_nonterm_ == grammar.max_nonterm_);
  }

  static bool CheckLongBasic() {
    ChomskyGrammar grammar;
    grammar.terminals_ = {-1};
    grammar.non_terminals_ = {2, 3};
    grammar.rules_[2] = {{2, 3, 3}};
    grammar.rules_[3] = {{-1}};
    grammar.max_nonterm_ = 3;
    grammar.min_term_ = -1;
    ChomskyGrammar::Builder builder(grammar);

    builder.HandleLong();

    ChomskyGrammar expected_grammar;
    expected_grammar.terminals_ = {-1};
    expected_grammar.non_terminals_ = {2, 3, 4};
    expected_grammar.rules_[2] = {{2, 4}};
    expected_grammar.rules_[4] = {{3, 3}};
    expected_grammar.rules_[3] = {{-1}};
    expected_grammar.max_nonterm_ = 4;
    expected_grammar.min_term_ = -1;

    return (expected_grammar.rules_ == grammar.rules_ &&
        expected_grammar.terminals_ == grammar.terminals_ &&
        expected_grammar.non_terminals_ == grammar.non_terminals_ &&
        expected_grammar.max_nonterm_ == grammar.max_nonterm_);
  }

  static bool CheckLong() {
    ChomskyGrammar grammar;
    grammar.terminals_ = {-1};
    grammar.non_terminals_ = {2, 3};
    grammar.rules_[2] = {{2, 3, 3, 3, 3}};
    grammar.rules_[3] = {{-1}};
    grammar.max_nonterm_ = 3;
    grammar.min_term_ = -1;
    ChomskyGrammar::Builder builder(grammar);

    builder.HandleLong();

    ChomskyGrammar expected_grammar;
    expected_grammar.terminals_ = {-1};
    expected_grammar.non_terminals_ = {2, 3, 4, 5, 6};
    expected_grammar.rules_[2] = {{2, 4}};
    expected_grammar.rules_[3] = {{-1}};
    expected_grammar.rules_[4] = {{3, 5}};
    expected_grammar.rules_[5] = {{3, 6}};
    expected_grammar.rules_[6] = {{3, 3}};
    expected_grammar.max_nonterm_ = 6;
    expected_grammar.min_term_ = -1;

    return (expected_grammar.rules_ == grammar.rules_ &&
        expected_grammar.terminals_ == grammar.terminals_ &&
        expected_grammar.non_terminals_ == grammar.non_terminals_ &&
        expected_grammar.max_nonterm_ == grammar.max_nonterm_);
  }

  static bool CheckEpsGenBasic() {
    ChomskyGrammar grammar;
    grammar.terminals_ = {-1};
    grammar.non_terminals_ = {2, 3};
    grammar.rules_[2] = {{2, 3}};
    grammar.rules_[3] = {{-1}, {0}};
    grammar.max_nonterm_ = 3;
    grammar.min_term_ = -1;
    ChomskyGrammar::Builder builder(grammar);

    builder.HandleEpsGenerative();

    ChomskyGrammar expected_grammar;
    expected_grammar.terminals_ = {-1};
    expected_grammar.non_terminals_ = {2, 3};
    expected_grammar.rules_[2] = {{2}, {2, 3}};
    expected_grammar.rules_[3] = {{-1}};
    expected_grammar.max_nonterm_ = 3;
    expected_grammar.min_term_ = -1;

    return (expected_grammar.rules_ == grammar.rules_ &&
        expected_grammar.terminals_ == grammar.terminals_ &&
        expected_grammar.non_terminals_ == grammar.non_terminals_ &&
        expected_grammar.max_nonterm_ == grammar.max_nonterm_);
  }


  static bool CheckEpsGen() {
    ChomskyGrammar grammar;
    grammar.terminals_ = {-1};
    grammar.non_terminals_ = {2, 3, 4, 5};
    grammar.rules_[2] = {{4, 3}, {4, 5}};
    grammar.rules_[3] = {{0}, {-1}};
    grammar.rules_[4] = {{0}, {-1}};
    grammar.rules_[5] = {{-1}};
    grammar.max_nonterm_ = 5;
    grammar.min_term_ = -1;
    ChomskyGrammar::Builder builder(grammar);

    builder.HandleEpsGenerative();

    ChomskyGrammar expected_grammar;
    expected_grammar.terminals_ = {-1};
    expected_grammar.non_terminals_ = {2, 3, 4, 5};
    expected_grammar.rules_[2] = {{0}, {3}, {4}, {4, 3}, {4, 5}, {5}};
    expected_grammar.rules_[3] = {{-1}};
    expected_grammar.rules_[4] = {{-1}};
    expected_grammar.rules_[5] = {{-1}};
    expected_grammar.max_nonterm_ = 5;
    expected_grammar.min_term_ = -1;

    return (expected_grammar.rules_ == grammar.rules_ &&
        expected_grammar.terminals_ == grammar.terminals_ &&
        expected_grammar.non_terminals_ == grammar.non_terminals_ &&
        expected_grammar.max_nonterm_ == grammar.max_nonterm_);
  }


  static bool CheckEmpty() {
    ChomskyGrammar grammar;
    grammar.terminals_ = {-1};
    grammar.non_terminals_ = {2, 3};
    grammar.rules_[2] = {{0}, {3, 3}};
    grammar.rules_[3] = {{-1}};
    grammar.max_nonterm_ = 3;
    grammar.min_term_ = -1;
    ChomskyGrammar::Builder builder(grammar);

    builder.HandleEmptyWord();

    ChomskyGrammar expected_grammar;
    expected_grammar.terminals_ = {-1};
    expected_grammar.non_terminals_ = {1, 2, 3};
    expected_grammar.rules_[1] = {{0}, {2}};
    expected_grammar.rules_[2] = {{3, 3}};
    expected_grammar.rules_[3] = {{-1}};
    expected_grammar.max_nonterm_ = 3;
    expected_grammar.min_term_ = -1;

    return (expected_grammar.rules_ == grammar.rules_ &&
        expected_grammar.terminals_ == grammar.terminals_ &&
        expected_grammar.non_terminals_ == grammar.non_terminals_ &&
        expected_grammar.max_nonterm_ == grammar.max_nonterm_);
  }

  static bool CheckOneLetterBasic() {
    ChomskyGrammar grammar;
    grammar.terminals_ = {-1};
    grammar.non_terminals_ = {1, 2, 3};
    grammar.rules_[1] = {{0}, {2}};
    grammar.rules_[2] = {{3}};
    grammar.rules_[3] = {{-1}};
    grammar.max_nonterm_ = 3;
    grammar.min_term_ = -1;
    ChomskyGrammar::Builder builder(grammar);

    builder.HandleOneLetterWord();

    ChomskyGrammar expected_grammar;
    expected_grammar.terminals_ = {-1};
    expected_grammar.non_terminals_ = {1, 2, 3};
    expected_grammar.rules_[1] = {{0}, {-1}};
    expected_grammar.rules_[2] = {{-1}};
    expected_grammar.rules_[3] = {{-1}};
    expected_grammar.max_nonterm_ = 3;
    expected_grammar.min_term_ = -1;

    return (expected_grammar.rules_ == grammar.rules_ &&
        expected_grammar.terminals_ == grammar.terminals_ &&
        expected_grammar.non_terminals_ == grammar.non_terminals_ &&
        expected_grammar.max_nonterm_ == grammar.max_nonterm_);
  }

  static bool CheckOneLetter() {
    ChomskyGrammar grammar;
    grammar.terminals_ = {-1};
    grammar.non_terminals_ = {1, 2, 3, 4};
    grammar.rules_[1] = {{0}, {2}};
    grammar.rules_[2] = {{3, 4}, {3}};
    grammar.rules_[3] = {{-1}};
    grammar.rules_[4] = {{3}};
    grammar.max_nonterm_ = 4;
    grammar.min_term_ = -1;
    ChomskyGrammar::Builder builder(grammar);

    builder.HandleOneLetterWord();

    ChomskyGrammar expected_grammar;
    expected_grammar.terminals_ = {-1};
    expected_grammar.non_terminals_ = {1, 2, 3, 4};
    expected_grammar.rules_[1] = {{0}, {-1}, {3, 4}};
    expected_grammar.rules_[2] = {{3, 4}, {-1}};
    expected_grammar.rules_[3] = {{-1}};
    expected_grammar.rules_[4] = {{-1}};
    expected_grammar.max_nonterm_ = 4;
    expected_grammar.min_term_ = -1;

    return (expected_grammar.rules_ == grammar.rules_ &&
        expected_grammar.terminals_ == grammar.terminals_ &&
        expected_grammar.non_terminals_ == grammar.non_terminals_ &&
        expected_grammar.max_nonterm_ == grammar.max_nonterm_);
  }

};

TEST(NonGen, NonGenBasic) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::NonGenerativeBasicTest());
}

TEST(NonGen, NonGenTest) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::NonGenerativeTest());
}

TEST(UnReach, UnReachableBasic) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::UnReachableBasicTest());
}

TEST(UnReach, UnReachableTest) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::UnReachableTest());
}

TEST(Mixed, MixedTestBasic) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::CheckMixedBasic());
}

TEST(Mixed, MixedTest1) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::CheckMixed());
}

TEST(Long, LongTestBasic) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::CheckLongBasic());
}

TEST(Long, LongTest) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::CheckLong());
}

TEST(EpsGen, EpsGenBasic) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::CheckEpsGenBasic());
}

TEST(EpsGen, EpsGenTest) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::CheckEpsGen());
}

TEST(Empty, EmptyWordTest) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::CheckEmpty());
}

TEST(OneLetter, OneLetterBasic) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::CheckOneLetterBasic());
}

TEST(OneLetter, OneLetterTest) {
  ASSERT_TRUE(ChomskyGrammar::BuildTester::CheckOneLetter());
}

