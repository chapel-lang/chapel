#include <stdio.h>
#define TOKENCOUNT_EXTERN
#include "tokencount.h"

int yyerror(char* str) {
  fprintf(stderr, "ERROR: %s\n", str);
  fprintf(stderr, "yytext is %s\n", yytext);
  exit(1);
}

int yywrap() {
}

int main(int argc, char* argv[]) {
  int i, j;

  fileVerbosity = PRINT_ALL ^ (PRINT_CODE | PRINT_TOKPLINE | PRINT_HIST);
  totVerbosity = PRINT_TOKENS;
#ifdef CHECKDIFF
  fileVerbosity = PRINT_CODE;
  totVerbosity = 0;
#endif
#ifdef CRON
  fileVerbosity = 0;
  totVerbosity = PRINT_QUIET | PRINT_TOKENS;
#endif
#ifdef DEBUG
  fileVerbosity = PRINT_ALL;
  totVerbosity = PRINT_ALL ^ PRINT_QUIET;
#endif

  clearGlob();

  for (i=1; i<argc; i++) {
    if (strchr(argv[i], '*') != NULL) {
      /* wildcard not expanded -- by sh when no matches exist, e.g. */
      break;
    }
    if (fileVerbosity & PRINT_FILESEP) {
      fflush(stdout);
      fprintf(stderr, "%s\n", argv[i]);
      fprintf(stderr, 
	      "=============================================================\n");
    }
    yyin = fopen(argv[i], "r");

    clearFile();
    clearLine();
    yyparse();
    if (strcmp(line, "") != 0) {
      handleLinefeed();
    }

    if (fileVerbosity & PRINT_FILESEP) {
      fflush(stdout);
      fprintf(stderr, 
	      "=============================================================\n");
    }
    if (fileVerbosity & PRINT_TOKENS) {
      fprintf(stderr, "TOKENS = %d\n", fileTokens);
    }
    if (fileVerbosity & PRINT_LINES) {
      fprintf(stderr, "LINES  = %d code, %d comment, %d blank\n", codeLines,
	      commentLines, blankLines);
    }
    if (fileVerbosity & PRINT_STATS) {
      fprintf(stderr, "MAX TOKENS/LINE = %d\n"
	      "AVG TOKENS/CODE LINE = %.2f\n", 
	      maxTokensPerLineInFile, (double)fileTokens/codeLines);
    }
    if (fileVerbosity & PRINT_HIST) {
      fprintf(stderr, "HISTOGRAM:\n");
      for (j=0; j<=maxTokensPerLineInFile; j++) {
	fprintf(stderr, "%3d: %3d\n", j, fileTokenHist[j]);
      }
    }
    if (fileVerbosity & PRINT_FILESEP) {
      fflush(stdout);
      fprintf(stderr, 
	      "=============================================================\n");
      fprintf(stderr, "\n");
    }
  }

  if (totVerbosity & PRINT_TOKENS) {
    if (totVerbosity & PRINT_QUIET) {
      fprintf(stdout, "%d\n", totTokens);
    } else {
      fprintf(stderr, "GRAND TOTAL = %d\n", totTokens);
    }
  }

  return 0;
}
