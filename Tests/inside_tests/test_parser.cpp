#include "Parser.hpp"
#include "gtest/gtest.h"

class ParseTester {
 public:
  static bool CheckPspBuild() {
    CF_Grammar G1("S->(S)S|;");

    CF_Grammar expected_grammar;
    expected_grammar.rules_[2] = {{-1, 2, -2, 2}, {0}};
    expected_grammar.terminals_ = {-1, -2};
    expected_grammar.non_terminals_ = {2};
    expected_grammar.max_nonterm_ = 2;
    return (G1.rules_ == expected_grammar.rules_ &&
        G1.terminals_ == expected_grammar.terminals_ &&
        G1.non_terminals_ == expected_grammar.non_terminals_ &&
        G1.max_nonterm_ == expected_grammar.max_nonterm_);
  }

  static bool CheckBalancedBuild() {
    CF_Grammar G2("S->aA|bB|;A->aAA|bS|a;B->bBB|aS|b;");

    CF_Grammar expected_grammar;
    expected_grammar.rules_[2] = {{-1, 3}, {-2, 4}, {0}};
    expected_grammar.rules_[3] = {{-1, 3, 3}, {-2, 2}, {-1}};
    expected_grammar.rules_[4] = {{-2, 4, 4}, {-1, 2}, {-2}};
    expected_grammar.terminals_ = {-1, -2};
    expected_grammar.non_terminals_ = {2, 3, 4};
    expected_grammar.max_nonterm_ = 4;
    return (G2.rules_ == expected_grammar.rules_ &&
        G2.terminals_ == expected_grammar.terminals_ &&
        G2.non_terminals_ == expected_grammar.non_terminals_ &&
        G2.max_nonterm_ == expected_grammar.max_nonterm_);
  }

  static bool CheckDiffPspBuild() {
    CF_Grammar G3("S->(S)S|[S]S|{S}S|;");

    CF_Grammar expected_grammar;
    expected_grammar.rules_[2] = {{-1, 2, -2, 2}, {-3, 2, -4, 2}, {-5, 2, -6, 2}, {0}};
    expected_grammar.terminals_ = {-1, -2, -3, -4, -5, -6};
    expected_grammar.non_terminals_ = {2};
    expected_grammar.max_nonterm_ = 2;
    return (G3.rules_ == expected_grammar.rules_ &&
        G3.terminals_ == expected_grammar.terminals_ &&
        G3.non_terminals_ == expected_grammar.non_terminals_ &&
        G3.max_nonterm_ == expected_grammar.max_nonterm_);
  }

  static bool CheckOtherBuild() {
    CF_Grammar G4("S->AB;A->BA|AB;B->aAa|B;");

    CF_Grammar expected_grammar;
    expected_grammar.rules_[2] = {{3, 4}};
    expected_grammar.rules_[3] = {{4, 3}, {3, 4}};
    expected_grammar.rules_[4] = {{-1, 3, -1}, {4}};
    expected_grammar.terminals_ = {-1};
    expected_grammar.non_terminals_ = {2, 3, 4};
    expected_grammar.max_nonterm_ = 4;
    return (G4.rules_ == expected_grammar.rules_ &&
        G4.terminals_ == expected_grammar.terminals_ &&
        G4.non_terminals_ == expected_grammar.non_terminals_ &&
        G4.max_nonterm_ == expected_grammar.max_nonterm_);
  }

  };

TEST(Psp, pspBuild) {
  ASSERT_TRUE(ParseTester::CheckPspBuild());
}

TEST(Balance, BalancedBuild) {
  ASSERT_TRUE(ParseTester::CheckBalancedBuild());
}

TEST(Diffpsp, BalancedBuild) {
  ASSERT_TRUE(ParseTester::CheckDiffPspBuild());
}

TEST(Other, BalancedBuild) {
  ASSERT_TRUE(ParseTester::CheckOtherBuild());
}