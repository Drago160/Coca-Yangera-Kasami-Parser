#include "CYK_Manager.hpp"

CYK_Manager::CYK_Manager(const CF_Grammar& Homski_Grammar) {
  G = Homski_Grammar;
  for (int N : G.GetNonTerms()) {
    dp.insert({N, {}});
  }
}

bool CYK_Manager::IsEmpty(const std::string& word) { return word.empty(); }

void CYK_Manager::InitDP(const std::string& word) {
  for (const auto& [src, r_parts] : G.GetRules()) {
    for (const auto& dst : r_parts) {
      if (dst[0] < 0) {
        for (size_t i = 0; i < word.size(); ++i) {
          if (G.GetCharCode(word[i]) == dst[0]) {
            dp[src].insert({i, i});
          }
        }
      }
    }
  }
}

void CYK_Manager::ProcessBlocks(size_t length, const std::string& word) {
  for (const auto& [src, r_parts] : G.GetRules()) {
    for (const auto& dst : r_parts) {
      if (dst.size() == 2) {
        for (size_t left = 0; left <= word.size() - length; ++left) {
          for (size_t bound = left; bound < left + length; ++bound) {
            if (dp[dst[0]].contains({left, bound}) &&
                dp[dst[1]].contains({bound + 1, left + length - 1})) {
              dp[src].insert({left, left + length - 1});
            }
          }
        }
      }
    }
  }
}

bool CYK_Manager::Recognize(const std::string& word) {
  if (IsEmpty(word)) {
    if (G.GetRules().at(1).contains({0})) {
      return true;
    }
    return false;
  }
  InitDP(word);
  for (size_t i = 2; i <= word.size(); ++i) {
    ProcessBlocks(i, word);
  }
  return dp[1].contains({0, word.size() - 1});
}