#include "stdchpl.h"

/*** GLOBAL VARIABLES ***/
static int gARGC;
static char** gARGV;

int setInCommandLine(char* varName, _integer64* value) {
  int i;
  int varSet = 0;

  // Start at 1 because gARGV[0] holds program name.
  for (i = 1; i < gARGC; i++) {
    char* currentArg = gARGV[i];
    int argLength = strlen(currentArg);
    int invalidArg = 1;
    char* varNamePtr = NULL;
    char* valuePtr = NULL;
    char* equalsSign = strchr(currentArg, '=');

    if ((argLength >= 2) &&   
        (currentArg[0] == '-') && (currentArg[1] == 's') && (equalsSign)) {
      // Add the null character to the end of what will be the varName pointer.
      *equalsSign  = '\0';  
      // The first two cells in the array hold "-s", the command-line flag.
      varNamePtr = currentArg + 2;
      valuePtr   = equalsSign + 1;
    } 

    if (varNamePtr && valuePtr) {
      if (strcmp(varNamePtr, varName) == 0) {
	if (sscanf(valuePtr, "%lld", value) == 1) {
	  invalidArg = 0;
	  varSet = 1;
	} 
      } else {
	invalidArg = 0;
      }
    }
    if (equalsSign) {
      *equalsSign = '=';
    }
    if (invalidArg) {
      fprintf(stderr, "***Error:  \"%s\" is not a valid argument***\n", currentArg);   
      exit(0);
    } 
  }
  return varSet;
}

int main(int argc, char* argv[]) {
  gARGC = argc;
  gARGV = argv;
  __init();

  return 0;
}
