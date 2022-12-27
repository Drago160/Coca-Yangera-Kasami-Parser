#include "Parser.hpp"

using word_code = std::vector<int>;

std::string CF_Grammar::Parser::ExtractSegm(size_t left, size_t right) {
  std::string segm;
  segm.resize(right - left + 1);
  for (size_t i = left; i <= right; ++i) {
    segm[i - left] = input_[i];
  }
  return segm;
}

void CF_Grammar::Parser::ReadRule() {
  char src = input_[read_idx];
  read_idx += 3; // ignore(->)
  size_t begin = read_idx;
  while (input_[read_idx] != ';') {
    if (input_[read_idx] == '|') {
      InsertNewRightPart(src, ExtractSegm(begin, read_idx - 1));
      begin = read_idx + 1;
    }
    ++read_idx;
  }
  InsertNewRightPart(src, ExtractSegm(begin, read_idx - 1));
  if (input_.size() != read_idx) {
    if (input_[read_idx] == '\n') {
      ++read_idx;
    }
  }
  ++read_idx;
}

void CF_Grammar::Parser::InsertNewRightPart(char src, const std::string& dst) {
  int src_code = GetCode(src);
  if (grammar_.rules_.contains(src_code)) {
    grammar_.rules_[src_code].insert(InCodeWord(dst));
  } else {
    grammar_.rules_.insert({src_code, {InCodeWord(dst)}});
  }
}

int CF_Grammar::Parser::GetCode(char c) {
  if (CF_Grammar::IsNonTerminal(c)) {
    if (grammar_.nonterminal_mapping_.contains(c)) {
      return grammar_.nonterminal_mapping_[c];
    } else {
      grammar_.nonterminal_mapping_[c] = ++(grammar_.max_nonterm_);
      return grammar_.nonterminal_mapping_[c];
    }
  }
  if (grammar_.terminal_mapping_.contains(c)) {
    return grammar_.terminal_mapping_[c];
  } else {
    grammar_.terminal_mapping_[c] = --grammar_.min_term_;
    return grammar_.terminal_mapping_[c];
  }
}

word_code CF_Grammar::Parser::InCodeWord(const std::string& str) {
  word_code str_code;
  for (char c : str) {
    str_code.push_back(GetCode(c));
  }
  if (str_code.empty()) {
    str_code.push_back(0); // eps
  }
  return str_code;
}

void CF_Grammar::Parser::HandleNonAndTerminals() {
  for (char c : input_) {
    if (c == '-' || c == '\n' || c == '|' || c == '>' || c == ' ' || c == ';') {
      continue;
    }
    if (grammar_.terminal_mapping_.contains(c)) {
      grammar_.terminals_.insert(GetCode(c));
    } else {
      grammar_.non_terminals_.insert(GetCode(c));
    }
  }
}

void CF_Grammar::Parser::ParseFormat() {
  grammar_.start_terminal_ = 2;
  grammar_.nonterminal_mapping_['S'] = 2;
  while (read_idx < input_.size() - 1) {
    ReadRule();
  }
  HandleNonAndTerminals();
}