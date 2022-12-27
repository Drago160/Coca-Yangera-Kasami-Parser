#include "CYK_Manager.hpp"

CYK_Manager::CYK_Manager(const ChomskyGrammar& Chomsky_Grammar) {
  G = Chomsky_Grammar;
}

bool CYK_Manager::IsEmpty(const std::string& word) { return word.empty(); }

void CYK_Manager::InitDP(const std::string& word) {
  for (const auto& [src, r_parts] : G.GetRules()) {
    for (const auto& dst : r_parts) {
      if (dst[0] >= 0) {
        continue;
      }
      for (size_t i = 0; i < word.size(); ++i) {
        if (G.GetCharCode(word[i]) == dst[0]) {
          correct_dests[src].insert({i, i});
        }
      }
    }
  }
}

void CYK_Manager::ProcessBlocks(size_t length, const std::string& word) {
  for (const auto& [src, r_parts] : G.GetRules()) { // src -> dst (A->B1C1|B2C2|...)
    for (const auto& dst : r_parts) {
      if (dst.size() != 2) {
        continue;
      }
      // дп по подотрезкам по факту
      for (size_t left = 0; left <= word.size() - length; ++left) { // left \in 0...(size-len)
        for (size_t bound = left; bound < left + length; ++bound) { // (left, bound, right)
          size_t right = left + length - 1;
          if (correct_dests[dst[0]].contains({left, bound}) && // Bi -> [left, bound]
              correct_dests[dst[1]].contains({bound + 1, right})) { // Ci -> [bound+1, right]
            correct_dests[src].insert({left, right}); // then A->[left, right]
          }
        }
      }
    }
  }
}

bool CYK_Manager::Recognize(const std::string& word) {
  if (IsEmpty(word)) {
    if (G.GetRules().at(1).contains({0})) { // S' -> eps
      return true;
    }
    return false;
  }
  InitDP(word);
  for (size_t i = 2; i <= word.size(); ++i) {
    ProcessBlocks(i, word);
  }
  return correct_dests[1].contains({0, word.size() - 1}); // S' -> w[0:size-1];
}