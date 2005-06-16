#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "files.h"
#include "misc.h"
#include "log.h"
#include "dump.h"
#include "runpasses.h"
#include "stringutil.h"
#include "symtab.h"
#include "../passes/pass.h"

bool printPasses = false;

struct PassInfo {
  char* name;
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


#define FIRST {NULL, NULL}
#define LAST {NULL, NULL}
#define RUN(x, a) {#x, a}
#include "passlist.h"



static void runPass(char* passName, Pass* pass, char* args) {
  struct timeval startTime;
  struct timeval stopTime;
  struct timezone timezone;
  char* view_filename = NULL;
  FILE* view_file = NULL;
  static int uid = 1;

  currentTraversal = copystring(passName);
  pass->setArgs(args);
  if (fdump_html) {
    if (strcmp(passName, "Verify")) {
      view_filename = glomstrings(3, "pass", intstring(uid++), ".html");
      fprintf(html_index_file, "<a href=\"%s\">%s</a><BR>\n", view_filename, passName);
    }
  }
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
  if (fdump_html) {
    if (strcmp(passName, "Verify")) {
      view_file = fopen(view_filename, "w");
      View* view = new View(false, view_file);
      view->setArgs(glomstrings(2, "html ", passName));
      view->run(Symboltable::getModuleList(MODULES_ALL));
      fclose(view_file);
    }
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
  if (fdump_html) {
    html_index_file = fopen(glomstrings(2, log_dir, "index.html"), "w");
    dump_index_header(html_index_file);
  }
  if (strcmp(passfilename, "") == 0) {
    PassInfo* pass = passlist+1;  // skip over FIRST
    
    while (pass->name != NULL) {
      runPass(pass->name, stringToPass(pass->name), pass->args);
      
      pass++;
    }
  } else {
    parsePassFile(passfilename);
  }
  if (fdump_html) {
    dump_index_footer(html_index_file);
    fclose(html_index_file);
  }
}
