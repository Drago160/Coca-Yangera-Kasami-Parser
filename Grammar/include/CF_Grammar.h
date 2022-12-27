#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <boost/dynamic_bitset.hpp>

const std::unordered_set<char> addition_alphabet = {'(', ')', '[', ']', '{', '}'};
class ParseTester;

struct Vector_hash {
  std::size_t operator()(const std::vector<int>& vec) const {
    if (vec.size() == 0) {
      return 0;
    } else {
      return std::hash<int>()(vec[0]) ^ vec.size();
    }
  }
};
class CF_Grammar {

 public:
  using word_code = std::vector<int>;
  using all_rights = std::unordered_set<word_code, Vector_hash>;

  CF_Grammar();
  CF_Grammar(std::unordered_set<char> non_terminals,  //  <N, Sigma, P, S>
             std::unordered_set<char> terminals,
             std::unordered_map<char, std::vector<std::string>> rules,
             char S = 'S');
  CF_Grammar(const std::string& rules); //  Input in format: A->str1|str2|...|strn;B->....
  CF_Grammar(const std::vector<char>& non_terminals,
             const std::vector<char>& terminals,
             const std::vector<std::vector<std::string>>& rules,
             char S);

  int GetStartNonTerm() const;
  void InsertRule(int src, const std::vector<int>& path);
  const std::unordered_map<int, all_rights>& GetRules() const;             // P
  int GetCharCode(char c);
  const std::unordered_set<int>& GetNonTerms();                       // N
  const std::unordered_set<int>& GetTerms();                       // N

 private:

  class Parser;

  class ChomskyBuilder {
   public:
    ChomskyBuilder(CF_Grammar& grammar) : grammar_(grammar) {}
   private:
    CF_Grammar& grammar_;
  };

 protected:

  static bool IsTerminal(char c);
  static bool IsNonTerminal(char c);

  int max_nonterm_ = 2;
  void RestoreNonTerminals();
  void CountMaxNonTerm();
  bool ContainRule(int src, const std::vector<int>& dst);

  std::unordered_set<int> terminals_;                           // Sigma
  std::unordered_set<int> non_terminals_;                       // N
  std::unordered_map<int, all_rights> rules_;                   // P
  std::unordered_map<char, int> terminal_mapping_;
  std::unordered_map<char, int> nonterminal_mapping_;
  int start_terminal_ = 2;                                          // S
  int min_term_ = 0;

  friend Parser;
  friend ParseTester;
};

class ChomskyGrammar : public CF_Grammar {
 public:
  ChomskyGrammar(const std::string& input);
  ChomskyGrammar();

 private:
  class Builder;
  friend Builder;
 public:
  class BuildTester;
  friend BuildTester;
};