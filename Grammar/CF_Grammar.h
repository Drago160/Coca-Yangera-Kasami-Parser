#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <boost/dynamic_bitset.hpp>

const std::unordered_set<char> addition_alphabet = {'(', ')', '[', ']', '{', '}'};

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
  using word_code = std::vector<int>;
  using all_rights = std::unordered_set<word_code, Vector_hash>;

 public:
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
  void ToHomski();
  void InsertRule(int src, const std::vector<int>& path);
  const std::unordered_map<int, all_rights>& GetRules() const;             // P
  int GetCharCode(char c);
  const std::unordered_set<int>& GetNonTerms();                       // N

 private:

  static bool IsTerminal(char c);
  static bool IsNonTerminal(char c);

  class Parser {
   public:
    Parser(const std::string input, CF_Grammar* grammar_ptr)
        : input_(input), grammar_ptr_(grammar_ptr) {};
    void ParseFormat();

   private:
    void ReadRule();
    int GetCode(char c);
    word_code InCodeWord(const std::string& str);
    void InsertNewRightPart(char src, const std::string& dst);
    std::string ExtractSegm(size_t left, size_t right);
    void HandleNonAndTerminals();
    CF_Grammar* grammar_ptr_;
    std::string input_;
    size_t read_idx = 0;
  };

  void RestoreNonTerminals();
  void CrateRightNonTerm(auto& src);
  void CountMaxNonTerm();
  bool ContainRule(int src, const std::vector<int>& dst);
  void SplitRule(CF_Grammar& grammar, std::vector<int> dst, int current);

  void DeleteNonGenerative();
  void DeleteUnReachable();
  void HandleMixed();
  void HandleLong();
  void HandleEpsGenerative();
  void HandleEmptyWord();
  void HandleOneLetterWord();

  void Rebuild();

  std::unordered_set<int> terminals_;                           // Sigma
  std::unordered_set<int> non_terminals_;                       // N
  std::unordered_map<int, all_rights> rules_;                   // P
  std::unordered_map<char, int> terminal_mapping_;
  std::unordered_map<char, int> nonterminal_mapping_;
  int start_terminal_ = 2;                                          // S
  int max_nonterm_ = 2;
  int min_term_ = 0;

  friend Parser;
};
