#pragma once
#include "CF_Grammar.h"

using Segm = std::pair<size_t, size_t>;

struct Segm_hash
{
  std::size_t operator () (const Segm segm) const
  {
    if (segm.first == segm.second) {
      return std::hash<size_t>()(segm.first);
    }
    return std::hash<size_t>()(segm.first)^std::hash<size_t>()(segm.second);
  }
};

class CYK_Manager {
 public:
  CYK_Manager (const ChomskyGrammar& grammar);
  bool Recognize(const std::string& word);
 private:
  bool IsEmpty(const std::string& word);
  void InitDP(const std::string& word);
  void ProcessBlocks(size_t length, const std::string& word);
  std::unordered_map<int, std::unordered_set<Segm, Segm_hash>> correct_dests; // correct_dests[A] = {{l, r} : A=>w[l:r]}
  CF_Grammar G;
};