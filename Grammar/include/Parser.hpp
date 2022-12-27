#pragma once
#include "CF_Grammar.h"


class CF_Grammar::Parser {
 public:
  Parser(const std::string input, CF_Grammar& grammar)
      : input_(input), grammar_(grammar) {};
  void ParseFormat();

 private:
  void ReadRule();
  int GetCode(char c);
  word_code InCodeWord(const std::string& str);
  void InsertNewRightPart(char src, const std::string& dst);
  std::string ExtractSegm(size_t left, size_t right);
  void HandleNonAndTerminals();
  CF_Grammar& grammar_;
  std::string input_;
  size_t read_idx = 0;

};