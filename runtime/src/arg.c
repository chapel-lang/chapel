#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"
#include "chplio.h"
#include "config.h"

static void parseString(char* currentArg) {
  char* equalsSign = NULL; 
  char* varName = NULL;
  char* value = NULL;

  equalsSign = strchr(currentArg, '=');	  
  if (equalsSign) {
    *equalsSign = '\0';
    varName = currentArg;
    value = equalsSign + 1;

    if (value) {
      if (*value == '\0') {
	fprintf(stderr, "***Error:  Configuration variable \"%s\" is missing"
		" its initialization value***\n", varName);
	exit(0);
      }
      installConfigVar(varName, value);
    } else {
      fprintf(stderr, "***Error:  \"%s\" is not a valid value***\n", value);
      exit(0);
    }
  } else {
    fprintf(stderr, "***Error:  \"%s\" is not a valid argument***\n", 
	    currentArg);
    exit(0);
  }
}

static int installedString(FILE* argFile, char* setConfigBuffer) {
  int stringLength = 0;
  char* equalsSign = strchr(setConfigBuffer, '=');
  stringLength = strlen(setConfigBuffer);
  char firstChar = '\0';
  char nextChar = '\0';

  if (!equalsSign || !(equalsSign + 1)) {
    return 0;
  }
  firstChar = equalsSign[1];
  if ((firstChar != '"') && (firstChar != '\'')) {
    return 0;
  }
  char* varName = setConfigBuffer;
  char* value = equalsSign + 2;
  *equalsSign = '\0';
  char lastChar = setConfigBuffer[stringLength - 1];
  
  if ((firstChar != lastChar) || (strlen(value) == 0)) {
    nextChar = fgetc(argFile);
    do {
      switch (nextChar) {
      case EOF:
	setConfigBuffer[stringLength] = '\0';
	fprintf(stderr, "***Error:  Found end of file while "
		"reading string: %c%s***\n", firstChar, value);
	exit(0);
	break;
	
      case '\n':
	setConfigBuffer[stringLength] = '\0';
	fprintf(stderr, "***Error:  Found newline while reading"
		" string: %c%s***\n", firstChar, value);
	exit(0);
	break;
	
      default:
	if (stringLength >= _default_string_length - 1) {
	  fprintf(stderr, "***Error:  String exceeds the "
		  "maximum string length of %d***\n", 
		  _default_string_length);
	  exit(0);
	}
	setConfigBuffer[stringLength] = nextChar;
	stringLength++;
	nextChar = fgetc(argFile);
      }
    } while (nextChar != firstChar);
  } else {
    stringLength--;
  }
  setConfigBuffer[stringLength] = '\0';
  installConfigVar(varName, value);
  return 1;
}


int parseArgs(int argc, char* argv[]) {
  int i;
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
	parseString(currentArg + 2);
	break;
      case 'f':
	{
	  char* argFilename = currentArg + 2;
	  FILE* argFile = fopen(argFilename, "r");
	  if (!argFile) {
	    fprintf(stderr, "***Error:  Unable to open %s***\n", argFilename);
	    exit(0);
	  } 
	  while (!feof(argFile)) {
	    int numScans = 0;
	    char setConfigBuffer[_default_string_length];
	    numScans = fscanf(argFile, _default_format_read_string, 
			      setConfigBuffer);
	    if (numScans == 1) {
	      if (!installedString(argFile, setConfigBuffer)) {
		parseString(setConfigBuffer);
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
