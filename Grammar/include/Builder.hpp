#pragma once
#include "CF_Grammar.h"

class ChomskyGrammar::Builder {
 public:
  Builder(ChomskyGrammar& grammar) : grammar_(grammar) {}
  void Build();
 private:

  static bool IsMixed(const word_code& dst);
  void CreateRightNonTerm(const ChomskyGrammar& grammar, auto& src);
  void SplitRule(ChomskyGrammar& grammar, word_code dst, int current);

  void DeleteNonGenerative();
  void DeleteUnReachable();
  void HandleMixed();
  void HandleLong();
  void HandleEpsGenerative();
  void HandleEmptyWord();
  void HandleOneLetterWord();

  ChomskyGrammar& grammar_;

  friend BuildTester;
};