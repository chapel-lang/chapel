#include <stdio.h>
#include <string.h>
#include "createAST.h"
#include "files.h"
#include "misc.h"
#include "stringutil.h"
#include "../passes/pass.h"

#define FIRST NULL
#define LAST NULL
#define RUN(x) new x()
#include "passlist.h"


#define REGISTER(pass) \
  if (strcmp(passname, #pass) == 0) { \
    return new pass(); \
  }


static Pass* stringToPass(char* passname) {
#include "passlist.cpp"  
  INT_FATAL("Couldn't find a pass named %s", passname);
  return NULL;
}


static void runPass(Pass* pass, Stmt* program, char* filename) {
  pass->filename = filename;
  pass->run(program);
}


static void parsePassFile(char* passfilename, char* filename) {
  FILE* passfile = openInputFile(passfilename);
  char passname[80];
  int readword;
  bool done;
  do {
    readword = fscanf(passfile, "%s", passname);
  } while (readword == 1 && strcmp(passname, "FIRST,") != 0);
  do {
    readword = fscanf(passfile, "%s", passname);
    done = strcmp(passname, "LAST") == 0;
    if (!done) {
      if (strncmp(passname, "RUN(", 4) != 0) {
	fail("ill-formed passname: %s", passname);
      }
      char* passnameStart = passname + 4; // 4 == strlen("RUN(")
      int passnameLen = strlen(passnameStart);
      passnameStart[passnameLen-2] = '\0';
      Pass* pass = stringToPass(passnameStart);
      runPass(pass, programStmts, filename);
    }
  } while (readword == 1 && !done);
  closeInputFile(passfile);
}


void runPasses(char* passfilename, char* filename) {
  if (strcmp(passfilename, "") == 0) {
    Pass** pass = passlist+1;  // skip over FIRST
    
    while ((*pass) != NULL) {
      runPass(*pass, programStmts, filename);
      
      pass++;
    }
  } else {
    parsePassFile(passfilename, filename);
  }
}


void DummyPass::run(Stmt* program) {
  fprintf(stdout, "Running dummy pass\n");
}
