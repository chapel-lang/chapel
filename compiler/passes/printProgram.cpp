#include "printProgram.h"
#include "stmt.h"

void PrintProgram::run(Stmt* program) {
  program->printList(stdout, "\n");
  printf("\n");
}
