/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include <cstring>
#include <cstdio>
#include <string>
#include "chpl/parsing/parser-stats.h"


namespace chpl {


namespace parsing {

void ParserStats::clearHist(int h[]) {
  int i;
  for (i = 0; i < HIST_SIZE; i++) {
    h[i] = 0;
  }
}

void ParserStats::clearLine() {
  if (firstLineInFile) {
    firstLineInFile = 0;
  } else {
    if (lineTokens < HIST_SIZE) {
      fileTokenHist[lineTokens]++;
      totTokenHist[lineTokens]++;
    }
  }
  if (lineTokens > maxTokensPerLineInFile)
    maxTokensPerLineInFile = lineTokens;
  if (lineTokens > maxTokensPerLineTot)
    maxTokensPerLineTot = lineTokens;
  lineTokens = 0;
  lineBlank = true;
  lineComment = true;
  if (printTokens) {
    line.clear();
  }
}

void ParserStats::clearFile() {
  fileTokens = 0;
  blankLines = 0;
  commentLines = 0;
  codeLines = 0;
  clearHist(fileTokenHist);
  maxTokensPerLineInFile = 0;
  firstLineInFile = 1;
}

void ParserStats::clearGlob() {
  totTokens = 0;
  clearHist(totTokenHist);
  maxTokensPerLineTot = 0;
}

void ParserStats::printSeparator() {
  fflush(stdout);
  fprintf(stderr,
          "=============================================================\n");
}

void ParserStats::printFileSummary() {
  if (printTokens) {
    int j;

    fflush(stdout);
    fprintf(stderr, "TOKENS = %d\n", fileTokens);
    fprintf(stderr, "LINES  = %d code, %d comment, %d blank\n", codeLines,
            commentLines, blankLines);
    fprintf(stderr, "MAX TOKENS/LINE = %d\n"
                    "AVG TOKENS/CODE LINE = %.2f\n",
            maxTokensPerLineInFile, (double) fileTokens / codeLines);
    fprintf(stderr, "HISTOGRAM:\n");
    for (j = 0; j <= maxTokensPerLineInFile; j++) {
      fprintf(stderr, "%3d: %3d\n", j, fileTokenHist[j]);
    }
  }
}

void ParserStats::initTokenCount() {
  if (printTokens && !countTokens) { // printTokens => countTokens
    countTokens = true;
  }
  if (countTokens) {
    clearGlob();
  }
}

void ParserStats::startCountingFileTokens(const char* filename) {
  bool firstCall = true;

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

void ParserStats::stopCountingFileTokens() {
  tokenCountingOn = false;

  if (printTokens) {
    if (!line.empty()) {
      // TODO: Can we reproduce or replace this call to processNewline?
      // processNewline(scanner);
      countNewline();
    }

    printSeparator();
    printFileSummary();
    printSeparator();
    fprintf(stderr, "\n");
  }
}

void ParserStats::finishCountingTokens() {
  if (countTokens) {
    if (printTokens) {
      fflush(stdout);
      fprintf(stderr, "GRAND TOTAL = ");
    }
    fprintf(stderr, "%d\n", totTokens);
  }
}

void ParserStats::countToken(const char* toktext1,
                             const char* toktext2,
                             const char* toktext3) {
  if (tokenCountingOn && countTokens) {
    if (printTokens) {
      line.push_back(' ');
      line.append(toktext1);
      if (toktext2 != NULL)
        line.append(toktext2);
      if (toktext3 != NULL)
        line.append(toktext3);
    }
    lineTokens++;
    fileTokens++;
    totTokens++;
    lineBlank = false;
    lineComment = false;
  }
}

void ParserStats::countNewline() {
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
      printf("%s\n", line.c_str());
    }
  }
  clearLine();
}

void ParserStats::countCommentLine() {
  if (tokenCountingOn && countTokens) {
    lineBlank = false;
  }
}

void ParserStats::countSingleLineComment(const char* comment) {
  if (printTokens) {
    if (!comment) {
      comment = "";
    }
    line.append(" ");
    line.append(comment);
  }
}

void ParserStats::countMultiLineComment(const char* comment) {
  if (printTokens) {
    // add space to tabify
    line.append(" ");
    if (!comment || strcmp(comment, "") == 0) {
      line.append("/* ");
      line.append(" */");
    } else {
      line.append(comment);
    }
  }
}

ParserStats::ParserStats(bool printTokens) {
  this->printTokens = printTokens;
  this->countTokens = true;
  this->blankLines = 0;
  this->lineTokens = 0;
  this->fileTokens = 0;
  this->totTokens = 0;
  this->blankLines = 0;
  this->commentLines = 0;
  this->codeLines = 0;
  this->maxTokensPerLineInFile = 0;
  this->maxTokensPerLineTot = 0;

  this->firstLineInFile = 0;
  this->lineBlank = 0;
  this->lineComment = 0;
}

ParserStats::ParserStats() {
  ParserStats(false);
}

} // end namespace parsing


} // end namespace chpl
