#include "createAST.h"
#include "filesToAST.h"

void FilesToAST::run(Stmt* program) {
  fileToAST(filename, 0);
}
