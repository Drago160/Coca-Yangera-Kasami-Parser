#include "CF_Grammar.h"
#include "CYK_Manager.hpp"
#include "iostream"

int main() {
  ChomskyGrammar G("S->a;");
  CYK_Manager m(G);
  std::cout << m.Recognize("a");
}