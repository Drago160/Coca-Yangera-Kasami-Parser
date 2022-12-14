#include "CF_Grammar.h"
#include "CYK_Manager.hpp"

int main() {
  CF_Grammar G("S->aA|bB;A->aC|bS;B->aS|bK;K->aB;C->bA||aR;R->aRR|bC;");
  G.ToHomski();
  return 0;
}