#include "CF_Grammar.h"

int CF_Grammar::GetStartNonTerm() const { return start_terminal_; }

const std::unordered_map<int, std::unordered_set<std::vector<int>, Vector_hash>>& CF_Grammar::GetRules() {
  return rules_;
};

int CF_Grammar::GetCharCode(char c) { return terminal_mapping_[c]; }

const std::unordered_set<int>& CF_Grammar::GetNonTerms() {
  return non_terminals_;
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

CF_Grammar::CF_Grammar(std::unordered_set<char> non_termianls,
                       std::unordered_set<char> terminals,
                       std::unordered_map<char, std::vector<std::string>> rules,
                       char S) {
  int terminal_code = -1;
  for (char c : terminals) {
    terminal_mapping_[c] = terminal_code;
    termianls_.insert(terminal_code);
    terminal_code--;
  }

  std::unordered_map<char, int> non_terminal_mapping;
  non_terminal_mapping[S] = 2;
  non_terminals_.insert(2);
  int non_terminal_code = 3;

  std::unordered_set<word_code, Vector_hash> empty_set;

  for (char N : non_termianls) {
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

bool CF_Grammar::IsTerminal(char c) {
  return ('a' <= c && c <= 'z') || addition_alphabet.contains(c);
}

bool CF_Grammar::IsNonTerminal(char c) {
  return ('A' <= c && c <= 'Z');
}

CF_Grammar::CF_Grammar(const std::string& rules) {
  start_terminal_ = 2;
  int term_code = -1;
  std::unordered_map<char, int> non_terminals_mapping;
  non_terminals_mapping['S'] = 2;
  non_terminals_.insert(2);
  int non_term_code = 3;
  for (char c : rules) {
    if (IsTerminal(c)) {
      if (!terminal_mapping_.contains(c)) {
        terminal_mapping_.insert({c, term_code});
        termianls_.insert(term_code);
        --term_code;
      }
    } else if (IsNonTerminal(c)) {
      if (!non_terminals_mapping.contains(c)) {
        non_terminals_mapping.insert({c, non_term_code});
        non_terminals_.insert(non_term_code);
        non_term_code++;
      }
    }
  }

  int curr_nonterm = non_terminals_mapping[rules[0]];

  // парсим формат
  for (size_t i = 0; i < rules.size(); ++i) {
    curr_nonterm = non_terminals_mapping[rules[i]];
    rules_.insert({curr_nonterm, {}});
    i += 3;
    if (rules[i] == ';') {
      InsertRule(curr_nonterm, {0});
    }
    while (rules[i] != ';') {
      std::vector<int> path;
      while (rules[i] != '|' && rules[i] != ';') {
        if (IsTerminal(rules[i])) {
          path.push_back(terminal_mapping_[rules[i]]);
        } else if (IsNonTerminal(rules[i])) {
          path.push_back(non_terminals_mapping[rules[i]]);
        }
        ++i;
      }
      if (rules[i] == '|') {
        if (rules[i + 1] == ';') {
          rules_[curr_nonterm].insert(path);
          rules_[curr_nonterm].insert({0});
          ++i;
          break;
        }
        ++i;
      }
      if (path.empty()) {
        path.push_back(0);
      }
      rules_[curr_nonterm].insert(path);
      if (i >= rules.size()) {
        break;
      }
    }
  }
  RestoreNonTerminals();
}

void CF_Grammar::CountMaxNonTerm() {
  max_non_term_ = 2;
  for (const auto& rule : rules_) {
    max_non_term_ = std::max(rule.first, max_non_term_);
  }
}

void CF_Grammar::RestoreNonTerminals() {
  non_terminals_.clear();
  for (const auto& [src, rules] : rules_) {
    non_terminals_.insert(src);
  }
}

void CF_Grammar::CrateRightNonTerm(auto& src) {
  for (const auto& rule : rules_) {
    src.insert({rule.first, {}});
    size_t cnt = 0;
    for (const auto& path : rule.second) {
      src[rule.first].push_back({});
      for (int symb : path) {
        if (symb > 0) {
          src[rule.first][cnt].insert(symb);
        }
      }
      ++cnt;
    }
  }
}

void CF_Grammar::DeleteNonGenerative() {
  std::unordered_set<int> generative;
  std::unordered_map<int, std::vector<std::unordered_set<int>>> right_non_terminals;
  CrateRightNonTerm(right_non_terminals);

  bool isChanged = true;
  bool isGen = true;
  while (isChanged) {
    isChanged = false;
    for (const auto& rule : rules_) { //  Перебираем все правила
      for (const auto&right_non_terms : right_non_terminals[rule.first]) { // Перебираем группы правых нетерминалов для каждого A -> w
        isGen = true;
        for (int Non_term : right_non_terms) { // преебираем нетерминалы правой части
          if (!generative.contains(Non_term)) { // если там есть непорождающие
            isGen = false;
            break;
          }
        }
        if (isGen) { // запишем его как порождающий
          if (!generative.contains(rule.first)) {
            generative.insert(rule.first);
            isChanged = true;
            break;
          }
        }
      }
    }
    RestoreNonTerminals();
  }

  auto non_gen = [&generative](int x) { return !generative.contains(x); };
  std::erase_if(non_terminals_, non_gen); // удалим непорождающие

  auto starts_from = [&generative](auto const& rule) {
    return !generative.contains(rule.first);
  };
  std::erase_if(rules_, starts_from); // удалим првила начинающиеся с непорождающих

  for (auto& rule : rules_) { // удалим правила откуда выводятся непорождающие
    auto include_non_gen = [&generative](auto const& path) {
      for (int symb : path) {
        if (symb > 0) {
          if (!generative.contains(symb)) {
            return true;
          }
        }
      }
      return false;
    };
    erase_if(rule.second, include_non_gen);
  }
  RestoreNonTerminals();
}

void CF_Grammar::DeleteUnReachable() {
  std::unordered_set<int> reachable;
  std::unordered_set<int> used;
  reachable.insert(start_terminal_);
  std::unordered_map<int, std::vector<std::unordered_set<int>>> right_non_terminals;
  CrateRightNonTerm(right_non_terminals);

  bool changes_flag = true;
  while (changes_flag) {
    changes_flag = false;
    for (int src : reachable) { // бежим по достежимым
      if (!used.contains(src)) { // если еще не смотрели на пути из него
        used.insert(src);
        for (const auto&non_terms : right_non_terminals[src]) { // смотрим на выводимые из него нетерминалы
          for (int non_terminal : non_terms) { // бежим по всем нетерминалам
            if (!reachable.contains(non_terminal)) {
              changes_flag = true;
              reachable.insert(non_terminal);
            }
          }
        }
      }
    }
  }

  auto starts_from = [&reachable](auto const& rule) {
    return !reachable.contains(rule.first);
  };
  std::erase_if(rules_, starts_from); // недостежимые удаляем
  RestoreNonTerminals();
}

bool IsMixed(const std::vector<int>& path) {
  bool include_term = false;
  bool include_non_term = false;
  for (int symb : path) {
    if (symb > 0) {
      include_non_term = true;
    } else if (symb < 0) {
      include_term = true;
    }
  }
  if (path.size() > 1 && include_term) { // abcd.. - (если более одного терминала) - смешанное
    return true;
  }
  return (include_term && include_non_term);
}

void CF_Grammar::HandleMixed() {
  CountMaxNonTerm();
  CF_Grammar new_Grammar;

  std::unordered_map<int, int> term_to_new_non_term;

  for (int term : termianls_) {
    ++max_non_term_;
    term_to_new_non_term[term] = max_non_term_;
    InsertRule(max_non_term_, {term});
    non_terminals_.insert(max_non_term_);
  }

  for (auto& [src, r_parts] : rules_) {
    for (auto& path : r_parts) {
      word_code new_path = path;
      if (IsMixed(path)) { // если смешанное правило делаем новые
        for (auto iter = new_path.begin(); iter != new_path.end(); ++iter) {
          if ((*iter) < 0) {
            (*iter) = term_to_new_non_term[(*iter)];
          }
        }
      }
      new_Grammar.InsertRule(src, new_path);
    }
  }

  new_Grammar.terminal_mapping_ = terminal_mapping_;
  new_Grammar.termianls_ = termianls_;
  new_Grammar.start_terminal_ = start_terminal_;
  (*this) = new_Grammar;
  RestoreNonTerminals();
  CountMaxNonTerm();
}

void SplitRule(CF_Grammar& G, std::vector<int> dst, int current) {
  size_t i = 0;
  while (dst.size() - i > 2) {
    ++G.max_non_term_;
    G.InsertRule(current, {dst[i], G.max_non_term_});
    current = G.max_non_term_;
    ++i;
  }
  G.InsertRule(current, {dst[i], dst[i + 1]});
}

void CF_Grammar::HandleLong() {
  CF_Grammar new_Grammar;
  new_Grammar.max_non_term_ = max_non_term_;
  for (const auto& [src, r_parts] : rules_) { // Перебираем правила B->w1|w2|...
    for (const auto& dst : r_parts) { // перебираем w1, w2, ...
      if (dst.size() > 2) {
        SplitRule(new_Grammar, dst, src);
      } else {
        new_Grammar.InsertRule(src, dst);
      }
    }
  }


  new_Grammar.terminal_mapping_ = terminal_mapping_;
  new_Grammar.termianls_ = termianls_;
  new_Grammar.start_terminal_ = start_terminal_;
  *this = new_Grammar;
  RestoreNonTerminals();
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

void CF_Grammar::HandleEpsGenerative() {
  boost::dynamic_bitset<> eps_gen_mask(max_non_term_ + 1, 0);

  for (const auto& [src, rule] : rules_) { // те что выводят eps за шаг
    if (rule.contains({0})) {
      eps_gen_mask[src] = true;
    }
  }

  CF_Grammar new_Grammar;
  bool change_flag = true;

  while (change_flag) {
    change_flag = false;
    for (const auto& [src, r_parts] : rules_) {
      for (const auto& path : r_parts) {
        if (path[0] < 0) { // добавляем правила вида A->a
          new_Grammar.InsertRule(src, {path[0]});
          continue;
        }
        if (path.size() == 1) { //  A->B
          if (eps_gen_mask.test(path[0])) {  // if B => eps
            if (!eps_gen_mask.test(src)) { // if A пока не считался eps - выводящим
              change_flag = true;
              eps_gen_mask[src] = true;
              change_flag = true;
            }
          } else {
            new_Grammar.InsertRule(src, path);
          }
          continue;
        }
        // A->BC
        bool first_change_flag = false;
        bool second_change_flag = false;
        if (eps_gen_mask.test(path[0]) // in A->BC: B => eps, то добавим A->C, если раньше не сделали
            && !new_Grammar.ContainRule(src, {path[1]})) {
          new_Grammar.InsertRule(src, {path[1]});
          first_change_flag = true;
        }
        if (eps_gen_mask.test(path[1]) // in A->BC: C => eps, то добавим A->B , если раньше не сделали
            && !new_Grammar.ContainRule(src, {path[0]})) {
          new_Grammar.InsertRule(src, {path[0]});
          second_change_flag = true;
        }
        change_flag = first_change_flag || second_change_flag; // if B or C => eps и не рассмотрели ранее
        if (first_change_flag && second_change_flag) {
          eps_gen_mask[src] = true;
        }
        new_Grammar.InsertRule(src, path); // add A->BC
      }
    }
  }

  auto only_eps = [](auto const& rule) {
    if (rule.second.size() == 1) {
      if (rule.second.contains({0})) {
        return true;
      }
    }
    return false;
  };
  erase_if(new_Grammar.rules_, only_eps);

  auto is_eps = [](auto const& dst) {
    return (dst.size() == 1 && dst[0] == 0);
  };
  for (auto& [src, r_parts] : new_Grammar.rules_) {
    erase_if(r_parts, is_eps);
  }

  if (eps_gen_mask.test(start_terminal_)) {
    new_Grammar.InsertRule(start_terminal_, {0});
  }
  new_Grammar.terminal_mapping_ = terminal_mapping_;
  new_Grammar.termianls_ = termianls_;
  new_Grammar.start_terminal_ = start_terminal_;
  *this = new_Grammar;
  RestoreNonTerminals();
}

void CF_Grammar::HandleEmptyWord() {
  start_terminal_ = 1; //  S'
  InsertRule(1, {2});  //  add S'->S
  if (ContainRule(2, {0})) {
    rules_[2].erase(std::vector<int>(1) = {0});
    InsertRule(1, {0});
  }
}

void CF_Grammar::HandleOneLetterWord() {
  CF_Grammar new_Grammar;
  std::unordered_map<int, std::vector<int>> one_letter_ways;
  //  add rules A->BC, A->a, S->eps
  for (const auto& rule : rules_) {
    for (const auto& dst : rule.second) {
      if (dst.size() == 2 || dst[0] <= 0) {
        new_Grammar.InsertRule(rule.first, dst);
      } else {
        one_letter_ways[rule.first].push_back(dst[0]);
      }
    }
  }

  bool is_changed = true;
  while (is_changed) {
    is_changed = false;
    for (const auto& rule : one_letter_ways) { // смотрим на правила A->B|C|D..
      for (const auto& way : rule.second) { //  бежим по B, C, D...
        if (new_Grammar.rules_.contains(way)) { // если есть B->B1B2|b...
          for (const auto& last_rule : new_Grammar.rules_[way]) { // перебираем B1B2,...,b...
            if (!new_Grammar.ContainRule(rule.first, last_rule)) {
              new_Grammar.InsertRule(rule.first,
                                     last_rule); // Добавим A->B1B2,... A->c... A->C1C2...
              is_changed = true;
            }
          }
        }
      }
    }
  }

  new_Grammar.terminal_mapping_ = terminal_mapping_;
  new_Grammar.termianls_ = termianls_;
  new_Grammar.start_terminal_ = start_terminal_;
  *this = new_Grammar;
  RestoreNonTerminals();
}



void CF_Grammar::ToHomski() {
  DeleteNonGenerative();
  DeleteUnReachable();
  HandleMixed();
  HandleLong();
  HandleEpsGenerative();
  HandleEmptyWord();
  HandleOneLetterWord();
  DeleteNonGenerative();
  DeleteUnReachable();
}

