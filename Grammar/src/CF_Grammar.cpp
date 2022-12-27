#include "CF_Grammar.h"
#include "Parser.hpp"
using word_code = std::vector<int>;
using all_rights = std::unordered_set<word_code, Vector_hash>;

int CF_Grammar::GetStartNonTerm() const { return start_terminal_; }

const std::unordered_map<int, all_rights>& CF_Grammar::GetRules() const {
  return rules_;
}

bool CF_Grammar::IsNonTerminal(char c) {
  return (c >= 'A' && c <= 'Z');
}

bool CF_Grammar::IsTerminal(char c) {
  return !IsNonTerminal(c);
}

int CF_Grammar::GetCharCode(char c) { return terminal_mapping_[c]; }

const std::unordered_set<int>& CF_Grammar::GetNonTerms() {
  return non_terminals_;
}

const std::unordered_set<int>& CF_Grammar::GetTerms() {
  return terminals_;
}

CF_Grammar::CF_Grammar() {}

void CF_Grammar::InsertRule(int src, const std::vector<int>& path) {
  if (rules_.contains(src)) {
    rules_[src].insert(path);
  } else {
    rules_.insert({src, {}});
    rules_[src].insert(path);
  }
}

CF_Grammar::CF_Grammar(std::unordered_set<char> non_terminals, std::unordered_set<char> terminals,
                       std::unordered_map<char, std::vector<std::string>> rules,
                       char S) {
  int terminal_code = -1;
  for (char c : terminals) {
    terminal_mapping_[c] = terminal_code;
    terminals_.insert(terminal_code);
    terminal_code--;
  }

  std::unordered_map<char, int> non_terminal_mapping;
  non_terminal_mapping[S] = 2;
  non_terminals_.insert(2);
  int non_terminal_code = 3;

  std::unordered_set<word_code, Vector_hash> empty_set;

  for (char N : non_terminals) {
    if (N != S) {
      non_terminal_mapping[N] = non_terminal_code;
      non_terminals_.insert(non_terminal_code);
      non_terminal_code++;
    }
    rules_.insert({N, empty_set});
  }

  for (const auto& [src, r_parts] : rules) { //  Перебираем A->w1|w2|...
    for (const auto& str_path : r_parts) { // перебираем такие wi
      word_code path;
      if (str_path.empty()) { // пустые слова кодируем нулем
        path.push_back(0);
      }
      for (char c : str_path) {
        if (terminals.contains(c)) {
          path.push_back(terminal_mapping_[c]);
        } else {
          path.push_back(non_terminal_mapping[c]);
        }
      }
      rules_[src].insert(path);
    }
  }
}

CF_Grammar::CF_Grammar(const std::string& rules) {
  Parser builder(rules, *this);
  builder.ParseFormat();
  RestoreNonTerminals();
}

void CF_Grammar::CountMaxNonTerm() {
  max_nonterm_ = 2;
  for (int nonterm : non_terminals_) {
    max_nonterm_ = std::max(max_nonterm_, nonterm);
  }
  /*
  for (const auto& rule : rules_) {
    max_nonterm_ = std::max(rule.first, max_nonterm_);
  }
   */
}

void CF_Grammar::RestoreNonTerminals() {
  non_terminals_.clear();
  for (const auto& [src, rules] : rules_) {
    non_terminals_.insert(src);
  }
}

bool CF_Grammar::ContainRule(int src, const std::vector<int>& dst) {
  if (!rules_.contains(src)) {
    return false;
  }
  if (!rules_[src].contains(dst)) {
    return false;
  }
  return true;
}
