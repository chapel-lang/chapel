#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "files.h"
#include "misc.h"
#include "runpasses.h"
#include "stringutil.h"
#include "symtab.h"
#include "../passes/pass.h"

bool printPasses = false;

struct PassInfo {
  char* name;
  Pass* pass;
  char* args;
};


#define START_PASSLIST_REGISTRATION \
  static Pass* stringToPass(char* passname) {

#define STOP_PASSLIST_REGISTRATION \
  INT_FATAL("Couldn't find a pass named %s", passname); \
  return NULL; \
}

#define REGISTER(pass) \
  if (strcmp(passname, #pass) == 0) { \
    return new pass(); \
  }

#include "passlist.cpp"  


#define FIRST {NULL, NULL, NULL}
#define LAST {NULL, NULL, NULL}
#define RUN(x, a) {#x, new x(), a}
#include "passlist.h"



static void runPass(char* passName, Pass* pass, char* args) {
  struct timeval startTime;
  struct timeval stopTime;
  struct timezone timezone;

  pass->setArgs(args);
  if (printPasses) {
    fprintf(stderr, "%32s :", passName);
    fflush(stderr);
    gettimeofday(&startTime, &timezone);
  }
  pass->run(Symboltable::getModuleList(pass->whichModules));
  if (printPasses) {
    gettimeofday(&stopTime, &timezone);
    fprintf(stderr, "%8.3f seconds\n",  
	    ((double)((stopTime.tv_sec*1e6+stopTime.tv_usec) - 
		      (startTime.tv_sec*1e6+startTime.tv_usec))) / 1e6);
  }
}


static void parsePassFile(char* passfilename) {
  FILE* passfile = openInputFile(passfilename);
  char passname[80] = "";
  char args[80] = "";
  int readPass;
  bool readLastPass;
  do {
    readPass = fscanf(passfile, "%s", passname);
  } while (readPass == 1 && strcmp(passname, "FIRST,") != 0);
  do {
    readPass = fscanf(passfile, "%s", passname);
    readLastPass = strcmp(passname, "LAST") == 0;
    if (!readLastPass) {
      if (strncmp(passname, "//", 2) == 0) {
	char nextChar;
	do {
	  fscanf(passfile, "%c", &nextChar);
	} while (nextChar != '\n');
      } else if (strncmp(passname, "RUN(", 4) != 0) {
	fail("ill-formed passname: %s", passname);
      } else {
	char* passnameStart = passname + 4; // 4 == strlen("RUN(")
	int passnameLen = strlen(passnameStart);
	passnameStart[passnameLen-1] = '\0'; // overwrite comma
      
	bool readChar;
	char nextChar;
	do {
	  readChar = fscanf(passfile, "%c", &nextChar);
	} while (readChar == 1 && nextChar != '\"');
	int argLength = 0;
	do {
	  readChar = fscanf(passfile, "%c", &nextChar);
	  if (readChar) {
	    args[argLength++] = nextChar;
	  }
	} while (readChar == 1 && nextChar != '\"');
	args[--argLength] = '\0';
	char extraStuff[80];
	fscanf(passfile, "%s", extraStuff);
	if (strcmp(extraStuff, "),") != 0) {
	  fail("pass name ended poorly: %s", extraStuff);
	}

	Pass* pass = stringToPass(passnameStart);
	runPass(passnameStart, pass, args);
      }
    }
  } while (readPass == 1 && !readLastPass);
  closeInputFile(passfile);
}


void runPasses(char* passfilename) {
  if (strcmp(passfilename, "") == 0) {
    PassInfo* pass = passlist+1;  // skip over FIRST
    
    while (pass->name != NULL) {
      runPass(pass->name, pass->pass, pass->args);
      
      pass++;
    }
  } else {
    parsePassFile(passfilename);
  }
}
