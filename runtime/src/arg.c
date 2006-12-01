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

    {"-s, --<cfgVar>=<val>", "set the value of a config var", 'c'},    
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
  char lastHeaderType = '\0';

  while (flagList[i].flag) {
    int thisFlag = strlen(flagList[i].flag);
    if (longestFlag < thisFlag) {
      longestFlag = thisFlag;
    }
    i++;
  }

  i = 0;
  while (flagList[i].flag) {
    printHeaders(flagList[i].headerType, &lastHeaderType);
    fprintf(stdout, "  %-*s -- %s\n", longestFlag, flagList[i].flag, 
            flagList[i].description);
    i++;
  }
  fprintf(stdout, "\n");
}


static _int64 getIntArg(char* valueString, char* memFlag) {
  char extraChars;
  _int64 value = 0;  /* initialization is silly hack for freebsd */
  int numScans;
  char* message;

  if (!valueString || strcmp(valueString, "") == 0) {
    message = _glom_strings(3, "The \"", memFlag, "\" flag is missing "
                                  "its int input");
    printError(message);
  }
  numScans = sscanf(valueString, _default_format_read_int64"%c", 
                    &value, &extraChars);
  if (numScans != 1) {
    message = _glom_strings(2, valueString, " is not of int type");
    printError(message);
  }
  return value; 
}


static char* getStringArg(char* valueString, char* memFlag) {
  char* message;
  if (!valueString || strcmp(valueString, "") == 0) {
    message = _glom_strings(3, "The \"", memFlag, "\" flag is missing "
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

typedef enum _MemFlagType {MemMax, MemStat, MemTrack, MemThreshold, MemTrace, MOther} MemFlagType;

static int parseMemFlag(char* memFlag) {
  char* equalsSign;
  char* valueString;
  MemFlagType flag = MOther;
  if (strncmp(memFlag, "memmax", 6) == 0) {
    flag = MemMax;
  } else if (strncmp(memFlag, "memstat", 7) == 0) {
    flag = MemStat;
  } else if (strncmp(memFlag, "memtrack", 8) == 0) {
    flag = MemTrack;
  } else if (strncmp(memFlag, "memthreshold", 12) == 0) {
    flag = MemThreshold;
  } else if (strncmp(memFlag, "memtrace", 8) == 0) {
    flag = MemTrace;
  }

  if (flag == MOther) {
    return 0;
  }

  equalsSign = strchr(memFlag, '=');
  valueString = NULL;

  if (equalsSign) {
    *equalsSign = '\0';
    valueString = equalsSign + 1;
  }

  switch (flag) {
  case MemMax:
    {
      _int64 value;
      value = getIntArg(valueString, memFlag);
      setMemmax(value);
      break;
    }

  case MemStat:
    {
      exitIfEqualsSign(equalsSign, memFlag);
      setMemstat();
      break;
    }
   
  case MemTrack:
    {
      exitIfEqualsSign(equalsSign, memFlag);
      setMemtrack();
      break;
    }

  case MemThreshold:
    {
      _int64 value;
      value = getIntArg(valueString, memFlag);
      setMemthreshold(value);
      break;
    }

  case MemTrace:
    {
      valueString = getStringArg(valueString, memFlag);
      setMemtrace(valueString);
      break;
    }

  case MOther:
    return 0;  // should never actually get here
  }

  return 1;
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
          int isSingleArg = 1;
          if (strcmp(flag, "help") == 0) {
            printHelpMessage();
            break;
          }
          if (flag[0] == 'm' && parseMemFlag(flag)) {
            break;
          }
          if (argLength < 3) {
            char* message = _glom_strings(3, "\"", currentArg, 
                                          "\" is not a valid argument");
            printError(message);
          }
          addToConfigList(currentArg + 2, isSingleArg);
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
          int isSingleArg = 1;
          if (argLength < 3) {
            char* message = _glom_strings(3, "\"", currentArg, "\" is not a "
                                          "valid argument");
            printError(message);
          }
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
