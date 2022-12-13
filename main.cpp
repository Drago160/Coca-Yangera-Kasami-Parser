#include "CF_Grammar.h"
#include "CYK_Manager.hpp"

int main() {
  CF_Grammar G("S->ABC;A->;B->bB|;C->aCa|CCa|aS|;");
  return 0;
}