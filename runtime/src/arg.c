#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"
#include "config.h"

int parseArgs(int argc, char* argv[]) {
  int i;

  initConfigVarTable();

  for (i = 1; i < argc; i++) {
    char* currentArg = argv[i];
    int argLength    = strlen(currentArg);
    char* equalsSign = strchr(currentArg, '=');
    char* varName    = NULL;
    char* value      = NULL;

    if ((argLength >= 3) &&
	(currentArg[0] == '-') && (currentArg[1] == 's') && (equalsSign)) {
      *equalsSign = '\0';
      varName     = currentArg + 2;
      value       = equalsSign + 1;
      installConfigVar(varName, value);
    } else {
      fprintf(stderr, "***Error:  \"%s\" is not a valid argument***\n", currentArg);
      exit(0);
    }
  }
  return 0;
}
