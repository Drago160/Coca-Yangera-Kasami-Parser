#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <boost/dynamic_bitset.hpp>

const std::unordered_set<char> addition_alphabet = {'(', ')', '[', ']', '{', '}'};

struct Vector_hash
{
  std::size_t operator () (const std::vector<int>& vec) const
  {
    if (vec.size() == 0) {
      return 0;
    } else {
      return std::hash<int>()(vec[0]) ^ vec.size();
    }
  }
};


class CF_Grammar {
 public:
  CF_Grammar();
  CF_Grammar(std::unordered_set<char> non_termianls,  //  <N, Sigma, P, S>
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

  int max_non_term_ = 2;

  const std::unordered_map<int, std::unordered_set<std::vector<int>, Vector_hash>>& GetRules();             // P

  int GetCharCode(char c);


  const std::unordered_set<int>& GetNonTerms();                       // N

 private:


  bool IsTerminal(char c);
  bool IsNonTerminal(char c);

  void RestoreNonTerminals();
  void CrateRightNonTerm(auto& src);
  void CountMaxNonTerm();
  bool ContainRule(int src, const std::vector<int>& dst);

  void DeleteNonGenerative();
  void DeleteUnReachable();
  void HandleMixed();
  void HandleLong();
  void HandleEpsGenerative();
  void HandleEmptyWord();
  void HandleOneLetterWord();

  void Rebuild();

  using word_code = std::vector<int>;
  using all_rights = std::unordered_set<word_code, Vector_hash>;

  std::unordered_set<int> termianls_;                          // Sigma
  std::unordered_set<int> non_terminals_;                       // N
  std::unordered_map<int, all_rights> rules_;             // P
  std::unordered_map<char, int> terminal_mapping_;
  int start_terminal_;                                          // S
};
