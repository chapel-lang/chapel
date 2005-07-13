#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"
#include "chplio.h"
#include "chplmem.h"
#include "config.h"
#include "chplrt.h"
#include "error.h"



static void printHeaders(char thisType, char* lastType) {
  if (thisType != *lastType) {
    fprintf(stdout, "\n");
    switch (thisType) {
    case 'c':
      fprintf(stdout, "CONFIG VAR FLAGS:\n");
      fprintf(stdout, "=================\n");
      break;

    case 'g':
      fprintf(stdout, "FLAGS:\n");
      fprintf(stdout, "======\n");
      break;
      
    case 'm':
      fprintf(stdout, "MEMORY FLAGS:\n");
      fprintf(stdout, "=============\n");        
      break;
    }
    *lastType = thisType;
  }
}


void printHelpTable(void) {
  typedef struct _flagType {
    char* flag;
    char* description;
    char headerType;
  } flagType;

  static flagType flagList[] = {
    {"-h, --help", "print this message", 'g'},

    {"-s<configVar>=<value>", "set the value of a config var", 'c'},    
    {"-f<filename>", "read in a file of config var assignments", 'c'},

    {"--memmax=<n>", "simulate 'n' bytes of memory available", 'm'}, 
    {"--memstat", "print memory statistics", 'm'},
    {"--memtrack", "track dynamic memory usage using a table", 'm'},
    {"--memtrace=<filename>", "write memory trace to filename", 'm'},
    {"--memthreshold=<n>", "filter memtrace for sizes >= 'n' bytes", 'm'},

    {NULL, NULL}
  };

  int i = 0;
  int longestFlag = 0;
  while (flagList[i].flag) {
    int thisFlag = strlen(flagList[i].flag);
    if (longestFlag < thisFlag) {
      longestFlag = thisFlag;
    }
    i++;
  }

  i = 0;
  char lastHeaderType = '\0';
  while (flagList[i].flag) {
    printHeaders(flagList[i].headerType, &lastHeaderType);
    fprintf(stdout, "  %-*s -- %s\n", longestFlag, flagList[i].flag, 
            flagList[i].description);
    i++;
  }
  fprintf(stdout, "\n");
}


static _integer64 getIntArg(char* valueString, char* memFlag) {
  char extraChars;
  _integer64 value = 0;  /* initialization is silly hack for freebsd */

  if (!valueString || strcmp(valueString, "") == 0) {
    char* message = _glom_strings(3, "The \"", memFlag, "\" flag is missing "
                                  "its integer input");
    printError(message);
  }
  int numScans = sscanf(valueString, _default_format_read_integer64"%c", 
                        &value, &extraChars);
  if (numScans != 1) {
    char* message = _glom_strings(2, valueString, " is not of integer type");
    printError(message);
  }
  return value; 
}


static char* getStringArg(char* valueString, char* memFlag) {
  if (!valueString || strcmp(valueString, "") == 0) {
    char* message = _glom_strings(3, "The \"", memFlag, "\" flag is missing "
                                  "its string input");
    printError(message);
  }
  return valueString;
}


static void exitIfEqualsSign(char* equalsSign, char* memFlag) {
  if (equalsSign) {
    char* message = _glom_strings(3, "The ", memFlag, " flag takes no "
                                  "argument");
    printError(message);
  }
}


static void parseMemFlag(char* memFlag) {
  char* equalsSign = strchr(memFlag, '=');
  char* valueString = NULL;

  if (equalsSign) {
    *equalsSign = '\0';
    valueString = equalsSign + 1;
  } 

  if (strcmp(memFlag, "memmax") == 0) {
    _integer64 value;
    value = getIntArg(valueString, memFlag);
    setMemmax(value);

  } else if (strcmp(memFlag, "memstat") == 0) {
    exitIfEqualsSign(equalsSign, memFlag);
    setMemstat();

  } else if (strcmp(memFlag, "memtrack") == 0) {
    exitIfEqualsSign(equalsSign, memFlag);
    setMemtrack();

  } else if ((strcmp(memFlag, "memthreshold") == 0) ||
             (strcmp(memFlag, "memthresshold") == 0) ||
             (strcmp(memFlag, "memthreshhold") == 0)) {
    _integer64 value;
    value = getIntArg(valueString, memFlag);
    setMemthreshold(value);

  } else if (strcmp(memFlag, "memtrace") == 0) {
    valueString = getStringArg(valueString, memFlag);
    setMemtrace(valueString);
    
  } else {
    char* message = _glom_strings(3, "\"", memFlag, "\" is not a valid "
                                  "execution option");
    printError(message);
  }
}


void parseArgs(int argc, char* argv[]) {
  int i;
  
  for (i = 1; i < argc; i++) {
    int argLength = 0;
    char* currentArg = argv[i];
    argLength = strlen(currentArg);

    if (argLength < 2) {
      char* message = _glom_strings(3, "\"", currentArg, "\" is not a valid "
                                    "argument");
      printError(message);
    }
    
    switch (currentArg[0]) {
    case '-':
      switch (currentArg[1]) {
      case '-':
        {
          char* flag = currentArg + 2;
          if (strcmp(flag, "help") == 0) {
            printHelpMessage();
          } else if (flag[0] == 'm') {
            parseMemFlag(flag);
          } else {
            char* message = _glom_strings(3, "\"", currentArg, "\" is not a "
                                          "valid execution option");
            printError(message);
          }
          break;
        }

      case 'f':
        {
          int isSingleArg = 0;
          addToConfigList(currentArg + 2, isSingleArg);
          break;
        }

      case 's':
        {
          if (argLength < 3) {
            char* message = _glom_strings(3, "\"", currentArg, "\" is not a "
                                          "valid argument");
            printError(message);
          }
          int isSingleArg = 1;
          addToConfigList(currentArg + 2, isSingleArg);
          break;
        }

      case 'h':
        printHelpMessage();
        break;

      default:
        {
          char* message = _glom_strings(3, "Unexpected flag:  \"", currentArg, 
                                        "\"");
          printError(message);
        }
      }
    }
  }
}
