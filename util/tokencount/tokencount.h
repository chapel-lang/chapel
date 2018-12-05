#ifndef _TOKENCOUNT_H_
#define _TOKENCOUNT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TOKENCOUNT_EXTERN
#define TOKENCOUNT_EXTERN extern
#endif

#define HIST_SIZE 4096


TOKENCOUNT_EXTERN int lineTokens;
TOKENCOUNT_EXTERN int fileTokens;
TOKENCOUNT_EXTERN int totTokens;

TOKENCOUNT_EXTERN int blankLines;
TOKENCOUNT_EXTERN int commentLines;
TOKENCOUNT_EXTERN int codeLines;

TOKENCOUNT_EXTERN int lineBlank;
TOKENCOUNT_EXTERN int lineComment;

TOKENCOUNT_EXTERN char line[4096];

extern FILE* yyin;
extern char* yytext;
extern int yydebug;
extern int yylex(void);
extern int yyparse(void);
extern int yyerror(char*);

TOKENCOUNT_EXTERN int fileTokenHist[HIST_SIZE];
TOKENCOUNT_EXTERN int totTokenHist[HIST_SIZE];
TOKENCOUNT_EXTERN int maxTokensPerLineInFile;
TOKENCOUNT_EXTERN int maxTokensPerLineTot;

TOKENCOUNT_EXTERN int firstLineInFile;


#define PRINT_TOKENS   0x01
#define PRINT_LINES    0x02
#define PRINT_STATS    0x04
#define PRINT_HIST     0x08
#define PRINT_FILESEP  0x10
#define PRINT_TOKPLINE 0x20
#define PRINT_CODE     0x40
#define PRINT_QUIET    0x80
#define PRINT_ALL      0xff

TOKENCOUNT_EXTERN int fileVerbosity;
TOKENCOUNT_EXTERN int totVerbosity;

#define clearHist(h) \
  { \
    int i; \
    for (i=0; i<HIST_SIZE; i++) { \
      h[i] = 0; \
    } \
  }

#define clearLine() \
  if (firstLineInFile) { \
    firstLineInFile = 0; \
  } else { \
    fileTokenHist[lineTokens]++; \
    totTokenHist[lineTokens]++; \
  } \
  if (lineTokens > maxTokensPerLineInFile) maxTokensPerLineInFile = lineTokens; \
  if (lineTokens > maxTokensPerLineTot) maxTokensPerLineTot = lineTokens; \
  lineTokens = 0; \
  lineBlank = 1; \
  lineComment = 1; \
  sprintf(line, "%s", "")

#define clearFile() \
  fileTokens = 0; \
  blankLines = 0; \
  commentLines = 0; \
  codeLines = 0; \
  clearHist(fileTokenHist); \
  maxTokensPerLineInFile = 0; \
  firstLineInFile = 1

#define clearGlob() \
  totTokens = 0; \
  clearHist(totTokenHist); \
  maxTokensPerLineTot = 0

#define handleLinefeed() \
  if (lineBlank) { \
    blankLines++; \
  } else if (lineComment) { \
    commentLines++; \
  } else { \
    codeLines++; \
  } \
  if (fileVerbosity & PRINT_TOKPLINE) { \
    if (lineTokens) { \
      printf("%3d | ", lineTokens); \
    } else if (lineBlank) { \
      printf("*B* | "); \
    } else if (lineComment) { \
      printf("*C* | "); \
    } else { \
      printf("    | "); \
    } \
  } \
  if (fileVerbosity & PRINT_CODE) { \
    printf("%s\n", line); \
  } \
  clearLine()

#endif

