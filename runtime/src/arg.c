#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"
#include "chplio.h"
#include "config.h"

static int parseString(char* currentArg) {
  char* equalsSign = NULL; 
  char* varName = NULL;
  char* value = NULL;
  int validArg = 0;

  equalsSign = strchr(currentArg, '=');	  
  if (equalsSign) {
    *equalsSign = '\0';
    varName = currentArg;

    value = equalsSign + 1;
    if (value) {
      installConfigVar(varName, value);
      validArg = 1;
    }
  }
  return validArg;
}


int parseArgs(int argc, char* argv[]) {
  int i;
  int validArg = 0;

  initConfigVarTable();
  for (i = 1; i < argc; i++) {
    int argLength = 0;
    char* currentArg = argv[i];
    argLength = strlen(currentArg);
    if (argLength < 3) {
      fprintf(stderr, "***Error:  \"%s\" is not a valid argument***\n", 
	      currentArg);
      exit(0);
    }
    switch (currentArg[0]) {
    case '-':
      switch (currentArg[1]) {
      case 's':
	  validArg = parseString(currentArg + 2);
	  if (!validArg) {
	    fprintf(stderr, "***Error:  \"%s\" is not a valid argument***\n",
		    currentArg);
	    exit(0);
	  }
	  break;
      case 'f':
	{
	  int numScans = 0;
	  char setConfigBuffer[_default_string_length];
	  char* setConfigString = NULL;
	  char* argFilename = NULL;
	  FILE* argFile = NULL;
	  argFilename = currentArg + 2;
	  argFile = fopen(argFilename, "r");

	  if (!argFile) {
	    fprintf(stderr, "***Error:  Unable to open %s***\n", argFilename);
	    exit(0);
	  } else {
	    while (!feof(argFile)) {
	      numScans = fscanf(argFile, _default_format_read_string, 
				setConfigBuffer);
	      /*
		int stringLength = 0;
		char firstCharOfValue;
		char nextChar;
		char* equalsSign = NULL;
		stringLength = strlen(setConfigBuffer);
		equalsSign = strchr(setConfigBuffer, '=');
		
		if (equalsSign && (equalsSign + 1)) {
		firstCharOfValue = equalsSign[1];
		
		if ((firstCharOfValue == '"') || (firstCharOfValue == '\'')) {
		nextChar = fgetc(argFile);
		while (nextChar != EOF) {
		if (stringLength >= _default_string_length - 1) {
		fprintf(stderr, "***Error:  String exceeds the maximum "
		"string length of %d***\n", 
		_default_string_length);
		exit(0);
		}
		if (nextChar == firstCharOfValue) {
		setConfigBuffer[stringLength] = '\0';
		nextChar = EOF;
		} else {
		setConfigBuffer[stringLength] = nextChar;
		stringLength++;
		nextChar = fgetc(argFile);
		}
		}
		}
		}
	      */
	      if (numScans == 1) {
		setConfigString = NULL;
		_copy_string(&setConfigString, setConfigBuffer);
		validArg = parseString(setConfigString);
		if (!validArg) {
		  fprintf(stderr, "***Error:  \"%s\" is not a valid argument"
			  "***\n", setConfigString);
		  exit(0);
		}
	      } 
	    }
	  }
	  break;
	}
      default:
	fprintf(stderr, "***Error: unexpected flag: '%s'\n", currentArg);
	exit(0);
      }
    }
  }
  return 0;
}
