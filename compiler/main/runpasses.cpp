#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "files.h"
#include "misc.h"
#include "log.h"
#include "runpasses.h"
#include "stringutil.h"
#include "symtab.h"
#include "../passes/pass.h"

bool printPasses = false;

static bool skipFunctionResolution = false;

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

  if (no_infer) {
    if (skipFunctionResolution) {
      if (!strcmp(passName, "FunctionResolution"))
        skipFunctionResolution = false;
      return;
    }
  } else if (!strcmp(passName, "FunctionResolution"))
    return;

  currentTraversal = stringcpy(passName);
  pass->setArgs(args);
  if (fdump_html) {
    gettimeofday(&startTime, &timezone);
  }
  if (printPasses) {
    fprintf(stderr, "%32s :", passName);
    fflush(stderr);
    gettimeofday(&startTime, &timezone);
  }
  pass->run(Symboltable::getModules(pass->whichModules));
  if (printPasses) {
    gettimeofday(&stopTime, &timezone);
    fprintf(stderr, "%8.3f seconds\n",  
            ((double)((stopTime.tv_sec*1e6+stopTime.tv_usec) - 
                      (startTime.tv_sec*1e6+startTime.tv_usec))) / 1e6);
  }
  if (!strcmp(passName, "ScopeResolveSymbols")) {
    postScopeResolution = true;
  } else if (!strcmp(passName, "RunAnalysis")) {
    postAnalysis = true;
  } else if (!strcmp(passName, "FunctionResolution")) {
    skipFunctionResolution = true;
  }

  if (fdump_html) {
    gettimeofday(&stopTime, &timezone);
    fprintf(html_index_file, "<TR><TD>", passName);
    if (!strcmp(passName, "RunAnalysis")) {
      fprintf(html_index_file, "<A HREF=\"dump.html\">");
    }
    fprintf(html_index_file, "%s", passName);
    int analysis_pass = !strcmp(passName, "RunAnalysis");
    if (analysis_pass) {
      fprintf(html_index_file, "</A>");
    }
    fprintf(html_index_file, "</TD><TD>", passName);
    HtmlView* htmlview = new HtmlView(analysis_pass);
    htmlview->setArgs(stringcat("html ", passName));
    htmlview->run(Symboltable::getModules(MODULES_CODEGEN));
    fprintf(html_index_file, "</TD></TR>", passName);
    fflush(html_index_file);
  }

  Verify* verify = new Verify();
  verify->run(Symboltable::getModules(MODULES_ALL));
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


void dump_index_header(FILE* f) {
  fprintf(f, "<HTML>\n");
  fprintf(f, "<HEAD>\n");
  fprintf(f, "<TITLE> Compilation Dump </TITLE>\n");
  fprintf(f, "<SCRIPT SRC=\"%s/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", 
         system_dir);
  fprintf(f, "<LINK REL=\"stylesheet\" HREF=\"%s/etc/www/mktree.css\">", 
         system_dir);
  fprintf(f, "</HEAD>\n");
  fprintf(f, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
  fprintf(f, "Compilation Dump<br><br></span></big></big>\n");
  fprintf(f, "<div style=\"text-align: left;\">\n\n");
}


void dump_index_footer(FILE* f) {
  fprintf(f, "</HTML>\n");
}


void runPasses(char* passfilename) {
  if (fdump_html) {
    html_index_file = fopen(stringcat(log_dir, "index.html"), "w");
    dump_index_header(html_index_file);
    fprintf(html_index_file, "<TABLE CELLPADDING=\"0\" CELLSPACING=\"0\">");
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
    fprintf(html_index_file, "</TABLE>");
    dump_index_footer(html_index_file);
    fclose(html_index_file);
  }
}
