#include <string.h>
#include "countTokens.h"
#include "misc.h"
#include "processTokens.h"
#include "yy.h"


bool countTokens = false;
bool printTokens = false;


#define HIST_SIZE 4096

static int lineTokens = 0;
static int fileTokens = 0;
static int totTokens = 0;

static int blankLines = 0;
static int commentLines = 0;
static int codeLines = 0;

static int lineBlank = false;
static int lineComment = false;
static char line[4096] = "";

static int fileTokenHist[HIST_SIZE];
static int totTokenHist[HIST_SIZE];
static int maxTokensPerLineInFile = 0;
static int maxTokensPerLineTot = 0;

static int firstLineInFile;


static void clearHist(int h[]) {
  int i;
  for (i=0; i<HIST_SIZE; i++) {
    h[i] = 0;
  }
}


static void clearLine(void) {
  if (firstLineInFile) {
    firstLineInFile = 0;
  } else {
    fileTokenHist[lineTokens]++;
    totTokenHist[lineTokens]++;
  }
  if (lineTokens > maxTokensPerLineInFile) maxTokensPerLineInFile = lineTokens;
  if (lineTokens > maxTokensPerLineTot) maxTokensPerLineTot = lineTokens;
  lineTokens = 0;
  lineBlank = true;
  lineComment = true;
  if (printTokens) {
    sprintf(line, "%s", "");
  }
}

static void clearFile(void) {
  fileTokens = 0;
  blankLines = 0;
  commentLines = 0;
  codeLines = 0;
  clearHist(fileTokenHist);
  maxTokensPerLineInFile = 0;
  firstLineInFile = 1;
}

static void clearGlob(void) {
  totTokens = 0;
  clearHist(totTokenHist);
  maxTokensPerLineTot = 0;
}


static void printSeparator(void) {
  fflush(stdout);
  fprintf(stderr, 
          "=============================================================\n");
}


static void printFileSummary(void) {
  if (printTokens) {
    int j;

    fflush(stdout);
    fprintf(stderr, "TOKENS = %d\n", fileTokens);
    fprintf(stderr, "LINES  = %d code, %d comment, %d blank\n", codeLines,
            commentLines, blankLines);
    fprintf(stderr, "MAX TOKENS/LINE = %d\n"
            "AVG TOKENS/CODE LINE = %.2f\n", 
            maxTokensPerLineInFile, (double)fileTokens/codeLines);
    fprintf(stderr, "HISTOGRAM:\n");
    for (j=0; j<=maxTokensPerLineInFile; j++) {
      fprintf(stderr, "%3d: %3d\n", j, fileTokenHist[j]);
    }
  }
}


static void initTokenCount(void) {
  if (printTokens && !countTokens) { // printTokens => countTokens
    countTokens = true;
  }
  if (countTokens) {
    clearGlob();
  }
}


void startCountingFileTokens(char* filename) {
  static bool firstCall = true;

  if (firstCall) {
    firstCall = false;
    initTokenCount();
  }

  if (countTokens) {
    clearFile();
    clearLine();
    if (printTokens) {
      fprintf(stderr, "%s\n", filename);
      printSeparator();
    }
  }
}


void stopCountingFileTokens(void) {
  if (printTokens) {
    if (strcmp(line, "") != 0) {
      processNewline();
    }

    printSeparator();
    printFileSummary();
    printSeparator();
    fprintf(stderr, "\n");
  }
}


void finishCountingTokens(void) {
  if (countTokens) {
    if (printTokens) {
      fflush(stdout);
      fprintf(stderr, "GRAND TOTAL = ");
    }
    fprintf(stderr, "%d\n", totTokens);
    clean_exit(0);
  }
}


void countToken(char* text) {
  if (countTokens) {
    if (printTokens) {
      sprintf(line, "%s %s", line, text);
    }
    lineTokens++;
    fileTokens++;
    totTokens++;
    lineBlank = false;
    lineComment = false;
  }
}


void countNewline(void) {
  if (countTokens) {
    if (lineBlank) {
      blankLines++;
    } else if (lineComment) {
      commentLines++;
    } else {
      codeLines++;
    }
    if (printTokens) {
      if (lineTokens) {
        printf("%3d | ", lineTokens);
      } else if (lineBlank) {
        printf("*B* | ");
      } else if (lineComment) {
        printf("*C* | ");
      } else {
        printf("    | ");
      }
      printf("%s\n", line);
      clearLine();
    }
  }
}


void countCommentLine(void) {
  if (countTokens) {
    lineBlank = false;
  }
}


void countSingleLineComment(char* comment) {
  if (printTokens) {
    sprintf(line, "%s -- %s", line, comment);
  }
}


void countMultiLineComment(char* comment) {
  if (printTokens) {
    sprintf(line, "%s /* %s */", line, comment);
  }
}
