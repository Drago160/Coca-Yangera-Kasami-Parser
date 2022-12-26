#include "CF_Grammar.h"
#include "CYK_Manager.hpp"

int main() {
  CF_Grammar G("S->(S)S|;");

  int a  = 1;
  G.ToHomski();
  return 0;
}