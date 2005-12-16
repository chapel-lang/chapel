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
};

#define START_PASSLIST_REGISTRATION \
  static void (*stringToPass(char* passname))(void) {

#define STOP_PASSLIST_REGISTRATION \
  INT_FATAL("Couldn't find a pass named %s", passname); \
  return NULL; \
}

#define REGISTER(pass) \
  if (strcmp(passname, #pass) == 0) { \
    return &pass; \
  }

#include "passlist.cpp"  

#define FIRST {NULL}
#define LAST {NULL}
#define RUN(x) {#x}
#include "passlist.h"


static void runPass(char *passName, void (*pass)(void)) {
  struct timeval startTime;
  struct timeval stopTime;
  struct timezone timezone;

  if (no_infer) {
    if (skipFunctionResolution) {
      if (!strcmp(passName, "functionResolution"))
        skipFunctionResolution = false;
      return;
    }
  } else if (!strcmp(passName, "functionResolution"))
    return;

  currentTraversal = stringcpy(passName);
  if (fdump_html) {
    gettimeofday(&startTime, &timezone);
  }
  if (printPasses) {
    fprintf(stderr, "%32s :", passName);
    fflush(stderr);
    gettimeofday(&startTime, &timezone);
  }
  (*pass)();
  if (printPasses) {
    gettimeofday(&stopTime, &timezone);
    fprintf(stderr, "%8.3f seconds\n",  
            ((double)((stopTime.tv_sec*1e6+stopTime.tv_usec) - 
                      (startTime.tv_sec*1e6+startTime.tv_usec))) / 1e6);
  }
  if (!strcmp(passName, "functionResolution")) {
    skipFunctionResolution = true;
  }

  if (fdump_html) {
    gettimeofday(&stopTime, &timezone);
    fprintf(html_index_file, "<TR><TD>");
    if (!strcmp(passName, "runAnalysis")) {
      fprintf(html_index_file, "<A HREF=\"dump.html\">");
    }
    fprintf(html_index_file, "%s", passName);
    int analysis_pass = !strcmp(passName, "RunAnalysis");
    if (analysis_pass) {
      fprintf(html_index_file, "</A>");
    }
    fprintf(html_index_file, "</TD><TD>");
    HtmlView* htmlview = new HtmlView(analysis_pass);
    htmlview->run(Symboltable::getModules(MODULES_ALL));
    fprintf(html_index_file, "</TD></TR>");
    fflush(html_index_file);
  }

  Verify* verify = new Verify();
  verify->run(Symboltable::getModules(MODULES_ALL));
}


static void parsePassFile(char* passfilename) {
  FILE* passfile = openInputFile(passfilename);
  char passname[80] = "";
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
        passnameStart[passnameLen-2] = '\0'; // overwrite ),
        runPass(passnameStart, stringToPass(passnameStart));
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
      runPass(pass->name, stringToPass(pass->name));
      
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

void passlistTest(void) {
  printf("Passlist test successful\n");
}
