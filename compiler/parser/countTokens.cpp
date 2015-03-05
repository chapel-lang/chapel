/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "countTokens.h"

#include <cstring>

#include "bison-chapel.h"
#include "misc.h"

bool countTokens = false;
bool printTokens = false;


#define HIST_SIZE 4096
#define LINE_SIZE 4096

static int lineTokens = 0;
static int fileTokens = 0;
static int totTokens = 0;

static int blankLines = 0;
static int commentLines = 0;
static int codeLines = 0;

static int lineBlank = false;
static int lineComment = false;
static char line[LINE_SIZE] = "";

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


static void clearLine() {
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

static void clearFile() {
  fileTokens = 0;
  blankLines = 0;
  commentLines = 0;
  codeLines = 0;
  clearHist(fileTokenHist);
  maxTokensPerLineInFile = 0;
  firstLineInFile = 1;
}

static void clearGlob() {
  totTokens = 0;
  clearHist(totTokenHist);
  maxTokensPerLineTot = 0;
}


static void printSeparator() {
  fflush(stdout);
  fprintf(stderr,
          "=============================================================\n");
}


static void printFileSummary() {
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


static void initTokenCount() {
  if (printTokens && !countTokens) { // printTokens => countTokens
    countTokens = true;
  }
  if (countTokens) {
    clearGlob();
  }
}

static bool tokenCountingOn = false;

void startCountingFileTokens(const char* filename) {
  static bool firstCall = true;

  if (firstCall) {
    firstCall = false;
    initTokenCount();
  }

  tokenCountingOn = true;

  if (countTokens) {
    clearFile();
    clearLine();
    if (printTokens) {
      fprintf(stderr, "%s\n", filename);
      printSeparator();
    }
  }
}


void stopCountingFileTokens(yyscan_t scanner) {
  tokenCountingOn = false;

  if (printTokens) {
    if (strcmp(line, "") != 0) {
      processNewline(scanner);
    }

    printSeparator();
    printFileSummary();
    printSeparator();
    fprintf(stderr, "\n");
  }
}


void finishCountingTokens() {
  if (countTokens) {
    if (printTokens) {
      fflush(stdout);
      fprintf(stderr, "GRAND TOTAL = ");
    }
    fprintf(stderr, "%d\n", totTokens);
    clean_exit(0);
  }
}


void countToken(const char* text) {
  if (tokenCountingOn && countTokens) {
    if (printTokens) {
      int start = strlen(line);
      int attempted = snprintf(&line[start], LINE_SIZE - start, " %s", text);
      if (attempted >= LINE_SIZE - start) {
        INT_FATAL("line length overflow");
      }
    }
    lineTokens++;
    fileTokens++;
    totTokens++;
    lineBlank = false;
    lineComment = false;
  }
}


void countNewline() {
  if (tokenCountingOn && countTokens) {
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
    }
  }
  clearLine();
}


void countCommentLine() {
  if (tokenCountingOn && countTokens) {
    lineBlank = false;
  }
}


void countSingleLineComment(const char* comment) {
  if (printTokens) {
    if (!comment) {
      comment = "";
    }
    int start = strlen(line);
    int attempted =
      snprintf(&line[strlen(line)], LINE_SIZE - start, " // %s", comment);
    if (attempted >= LINE_SIZE - start) {
      INT_FATAL("line length overflow: %s", line);
    }
  }
}


void countMultiLineComment(const char* comment) {
  if (printTokens) {
    int start = strlen(line);
    int attempted =
      snprintf(&line[strlen(line)], LINE_SIZE - start, " /* %s */", comment);
    if (attempted >= LINE_SIZE - start) {
      INT_FATAL("line length overflow");
    }
  }
}
