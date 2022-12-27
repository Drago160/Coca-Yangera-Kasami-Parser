#include "Builder.hpp"

ChomskyGrammar::ChomskyGrammar(const std::string& input) : CF_Grammar(input) {
  Builder builder(*this);
  builder.Build();
}

ChomskyGrammar::ChomskyGrammar() : CF_Grammar() {}

void ChomskyGrammar::Builder::Build() {
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

void ChomskyGrammar::Builder::CreateRightNonTerm(const ChomskyGrammar& grammar, auto& src) {
  for (const auto& rule : grammar.rules_) {
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

void ChomskyGrammar::Builder::DeleteNonGenerative() {
  std::unordered_set<int> generative;
  std::unordered_map<int, std::vector<std::unordered_set<int>>> right_non_terminals;
  CreateRightNonTerm(grammar_, right_non_terminals);
  bool isChanged = true;
  bool isGen = true;

  for (const auto& [src, r_parts] : grammar_.rules_) {
    for (const auto& code : r_parts) {
      bool is_terminal_word = true;
      for (int c : code) {
        if (c > 0) {
          is_terminal_word = false;
          break;
        }
      }
      if (is_terminal_word) {
        generative.insert(src);
        break;
      }
    }
  }

  while (isChanged) {
    isChanged = false;
    for (const auto& rule : grammar_.rules_) { //  Перебираем все правила
      // Перебираем группы правых нетерминалов для каждого A -> w
      for (const auto& right_non_terms : right_non_terminals[rule.first]) {
        isGen = true;
        for (int Non_term : right_non_terms) { // преебираем нетерминалы правой части
          if (!generative.contains(Non_term)) { // если там есть непорождающие
            isGen = false;
            break;
          }
        }
        if (isGen && !generative.contains(rule.first)) { // запишем его как порождающий
          generative.insert(rule.first);
          isChanged = true;
          break;
        }
      }
    }
    grammar_.RestoreNonTerminals();
  }

  auto non_gen = [&generative](int x) { return !generative.contains(x); };
  std::erase_if(grammar_.non_terminals_, non_gen); // удалим непорождающие

  auto starts_from = [&generative](auto const& rule) {
    return !generative.contains(rule.first);
  };
  std::erase_if(grammar_.rules_, starts_from); // удалим првила начинающиеся с непорождающих

  for (auto& rule : grammar_.rules_) { // удалим правила откуда выводятся непорождающие
    auto include_non_gen = [&generative](auto const& path) {
      for (int symb : path) {
        if (symb > 0 && !generative.contains(symb)) {
          return true;
        }
      }
      return false;
    };
    erase_if(rule.second, include_non_gen);
  }
  grammar_.RestoreNonTerminals();
}


void ChomskyGrammar::Builder::DeleteUnReachable() {
  std::unordered_set<int> reachable;
  std::unordered_set<int> already_checked;
  reachable.insert(grammar_.start_terminal_);
  std::unordered_map<int, std::vector<std::unordered_set<int>>> right_non_terminals;
  CreateRightNonTerm(grammar_, right_non_terminals);

  bool changes_flag = true;
  while (changes_flag) {
    changes_flag = false;
    for (int src : reachable) { // бежим по достежимым
      if (already_checked.contains(src)) { // если уже не смотрели на пути из него
        continue;
      }
      already_checked.insert(src);
      // смотрим на выводимые из него нетерминалы
      for (const auto& non_terms : right_non_terminals[src]) {
        for (int non_terminal : non_terms) { // бежим по всем нетерминалам
          if (!reachable.contains(non_terminal)) {
            changes_flag = true;
            reachable.insert(non_terminal);
          }
        }
      }
    }
  }

  auto starts_from = [&reachable](auto const& rule) {
    return !reachable.contains(rule.first);
  };
  std::erase_if(grammar_.rules_, starts_from); // недостежимые удаляем
  grammar_.RestoreNonTerminals();
}

bool ChomskyGrammar::Builder::IsMixed(const std::vector<int>& path) {
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

void ChomskyGrammar::Builder::HandleMixed() {
  grammar_.CountMaxNonTerm();
  //grammar_.RestoreNonTerminals();
  ChomskyGrammar new_Grammar;
  std::unordered_map<int, int> term_to_new_non_term;

  for (int term : grammar_.terminals_) {
    ++grammar_.max_nonterm_;
    term_to_new_non_term[term] = grammar_.max_nonterm_;
    grammar_.InsertRule(grammar_.max_nonterm_, {term});
    grammar_.non_terminals_.insert(grammar_.max_nonterm_);
  }

  for (auto& [src, r_parts] : grammar_.rules_) {
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
  grammar_.rules_ = new_Grammar.rules_;
}


void ChomskyGrammar::Builder::SplitRule(ChomskyGrammar& grammar, std::vector<int> dst, int current) {
  size_t i = 0;
  while (dst.size() - i > 2) {
    ++grammar.max_nonterm_;
    grammar.non_terminals_.insert(grammar.max_nonterm_);
    grammar.InsertRule(current, {dst[i], grammar.max_nonterm_});
    current = grammar.max_nonterm_;
    ++i;
  }
  grammar.InsertRule(current, {dst[i], dst[i + 1]});
}

void ChomskyGrammar::Builder::HandleLong() {
  ChomskyGrammar new_Grammar;
  new_Grammar.max_nonterm_ = grammar_.max_nonterm_;
  for (const auto& [src, r_parts] : grammar_.rules_) { // Перебираем правила B->w1|w2|...
    for (const auto& dst : r_parts) { // перебираем w1, w2, ...
      if (dst.size() > 2) {
        SplitRule(new_Grammar, dst, src);
      } else {
        new_Grammar.InsertRule(src, dst);
      }
    }
  }
  grammar_.rules_ = new_Grammar.rules_;
  grammar_.RestoreNonTerminals();
  grammar_.CountMaxNonTerm();
}


void ChomskyGrammar::Builder::HandleEpsGenerative() {
  boost::dynamic_bitset<> eps_gen_mask(grammar_.max_nonterm_ + 1, 0);

  for (const auto& [src, rule] : grammar_.rules_) { // те что выводят eps за шаг
    if (rule.contains({0})) {
      eps_gen_mask[src] = true;
    }
  }

  ChomskyGrammar new_Grammar;
  bool change_flag = true;

  while (change_flag) {
    change_flag = false;
    for (const auto& [src, r_parts] : grammar_.rules_) {
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

        // in A->BC: B => eps, то добавим A->C, если раньше не сделали
        if (eps_gen_mask.test(path[0])
            && !new_Grammar.ContainRule(src, {path[1]})) {
          new_Grammar.InsertRule(src, {path[1]});
          first_change_flag = true;
        }

        // in A->BC: C => eps, то добавим A->B , если раньше не сделали
        if (eps_gen_mask.test(path[1])
            && !new_Grammar.ContainRule(src, {path[0]})) {
          new_Grammar.InsertRule(src, {path[0]});
          second_change_flag = true;
        }

        // if B or C => eps и не рассмотрели ранее
        change_flag = first_change_flag || second_change_flag;
        if (first_change_flag && second_change_flag) {
          eps_gen_mask[src] = true;
        }
        new_Grammar.InsertRule(src, path); // add A->BC
      }
    }
  }

  auto only_eps = [](auto const& rule) {
    if (rule.second.size() == 1 && rule.second.contains({0})) {
      return true;
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

  if (eps_gen_mask.test(grammar_.start_terminal_)) {
    new_Grammar.InsertRule(grammar_.start_terminal_, {0});
  }

  grammar_.rules_ = new_Grammar.rules_;
  grammar_.RestoreNonTerminals();
  grammar_.CountMaxNonTerm();
}


void ChomskyGrammar::Builder::HandleEmptyWord() {
  grammar_.start_terminal_ = 1; //  S'
  grammar_.InsertRule(1, {2});  //  add S'->S
  if (grammar_.ContainRule(2, {0})) {
    grammar_.rules_[2].erase(std::vector<int>(1) = {0});
    grammar_.InsertRule(1, {0});
  }
  grammar_.non_terminals_.insert(1);
}


void ChomskyGrammar::Builder::HandleOneLetterWord() {
  ChomskyGrammar new_Grammar;
  std::unordered_map<int, std::vector<int>> one_letter_ways;
  //  add rules A->BC, A->a, S->eps
  for (const auto& rule : grammar_.rules_) {
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
        if (!new_Grammar.rules_.contains(way)) { // если есть B->B1B2|b...
          continue;
        }
        for (const auto& last_rule : new_Grammar.rules_[way]) { // перебираем B1B2,...,b...
          if (!new_Grammar.ContainRule(rule.first, last_rule)) {
            new_Grammar.InsertRule(rule.first, last_rule); // Добавим A->B1B2,... A->c... A->C1C2...
            is_changed = true;
          }
        }
      }
    }
  }
  grammar_.rules_ = new_Grammar.rules_;
  grammar_.RestoreNonTerminals();
  grammar_.CountMaxNonTerm();
}