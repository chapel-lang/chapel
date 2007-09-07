#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"
#include "chplexit.h"
#include "chplio.h"
#include "chplmem.h"
#include "chplrt.h"
#include "config.h"
#include "error.h"
#include "gdb.h"


static int gdbFlag = 0;

int _runInGDB(void) {
  return gdbFlag;
}

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


static void printHelpTable(void) {
  typedef struct _flagType {
    const char* flag;
    const char* description;
    const char headerType;
  } flagType;

  static flagType flagList[] = {
    {"-h, --help", "print this message", 'g'},
    {"-nl <n>", "run program using n locales", 'g'},
    {"", "(equivalent to setting the numLocales config const)", 'g'},
    {"-v, --verbose", "run program in verbose mode", 'g'},
    {"--gdb", "run program in gdb", 'g'},

    {"-s, --<cfgVar>=<val>", "set the value of a config var", 'c'},    
    {"-f<filename>", "read in a file of config var assignments", 'c'},

    {"--memmax=<n>", "simulate 'n' bytes of memory available", 'm'}, 
    {"--memstat", "print memory statistics", 'm'},
    {"--memtrack", "track dynamic memory usage using a table", 'm'},
    {"--memtrace=<filename>", "write memory trace to filename", 'm'},
    {"--memthreshold=<n>", "filter memtrace for sizes >= 'n' bytes", 'm'},

    {NULL, NULL, ' '}
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
    if (flagList[i].flag[0] == '\0') {
      fprintf(stdout, "  %-*s    %s\n", longestFlag, flagList[i].flag,
              flagList[i].description);
    } else {
      fprintf(stdout, "  %-*s  : %s\n", longestFlag, flagList[i].flag, 
              flagList[i].description);
    }
    i++;
  }
  fprintf(stdout, "\n");
}


static _int64 getIntArg(char* valueString, const char* memFlag) {
  char extraChars;
  _int64 value = 0;  /* initialization is silly hack for freebsd */
  int numScans;
  char* message;

  if (!valueString || strcmp(valueString, "") == 0) {
    message = _glom_strings(3, "The ", memFlag, " flag is missing "
                                  "its int input");
    _printError(message, 0, 0);
  }
  numScans = sscanf(valueString, _default_format_read_int64"%c", 
                    &value, &extraChars);
  if (numScans != 1) {
    message = _glom_strings(2, valueString, " is not of int type");
    _printError(message, 0, 0);
  }
  return value; 
}


static char* getStringArg(char* valueString, const char* memFlag) {
  char* message;
  if (!valueString || strcmp(valueString, "") == 0) {
    message = _glom_strings(3, "The ", memFlag, " flag is missing "
                                  "its string input");
    _printError(message, 0, 0);
  }
  return valueString;
}


static void exitIfEqualsSign(char* equalsSign, const char* memFlag) {
  if (equalsSign) {
    char* message = _glom_strings(3, "The ", memFlag, " flag takes no "
                                  "argument");
    _printError(message, 0, 0);
  }
}

typedef enum _MemFlagType {MemMax, MemStat, MemTrack, MemThreshold, MemTrace, MOther} MemFlagType;

static int parseMemFlag(const char* memFlag) {
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
    valueString = equalsSign + 1;
  }

  switch (flag) {
  case MemMax:
    {
      _int64 value;
      value = getIntArg(valueString, "--memmax");
      setMemmax(value);
      break;
    }

  case MemStat:
    {
      exitIfEqualsSign(equalsSign, "--memstat");
      setMemstat();
      break;
    }
   
  case MemTrack:
    {
      exitIfEqualsSign(equalsSign, "--memstat");
      setMemtrack();
      break;
    }

  case MemThreshold:
    {
      _int64 value;
      value = getIntArg(valueString, "--memthreshold");
      setMemthreshold(value);
      break;
    }

  case MemTrace:
    {
      valueString = getStringArg(valueString, "--memtrace");
      setMemtrace(valueString);
      break;
    }

  case MOther:
    return 0;  // should never actually get here
  }

  return 1;
}


static void unexpectedArg(const char* currentArg) {
  char* message = _glom_strings(3, "Unexpected flag:  \"", currentArg, "\"");
  _printError(message, 0, 0);
}


static _int32 _argNumLocales = 0;

static void parseNumLocales(const char* numPtr) {
  char* stopPtr;
  _argNumLocales = strtol(numPtr, &stopPtr, 10);
  if (*stopPtr != '\0') {
    char* message = _glom_strings(3, "\"", numPtr, 
                                  "\" is not a valid number of locales"
                                  );
    _printError(message, 0, 0);
  }
  if (_argNumLocales < 1) {
    _printError("Number of locales must be greater than 0", 0, 0);
  }
}


_int32 getArgNumLocales(void) {
  return _argNumLocales;
}


void parseArgs(int argc, char* argv[]) {
  int i;
  int printHelp = 0;

  for (i = 1; i < argc; i++) {
    int argLength = 0;
    const char* currentArg = argv[i];
    argLength = strlen(currentArg);

    if (argLength < 2) {
      const char* message = _glom_strings(3, "\"", currentArg, 
                                          "\" is not a valid argument");
      _printError(message, 0, 0);
    }

    switch (currentArg[0]) {
    case '-':
      switch (currentArg[1]) {
      case '-':
        {
          const char* flag = currentArg + 2;
          int isSingleArg = 1;

          if (strcmp(flag, "gdb") == 0) {
            gdbFlag = i;
            break;
          }
            
          if (strcmp(flag, "help") == 0) {
            printHelp = 1;
            break;
          }
          if (strcmp(flag, "verbose") == 0) {
            verbosity=2;
            break;
          }
          if (strncmp(flag, "numLocales", 10) == 0) {
            if (flag[10] == '=') {
              parseNumLocales(&(flag[11]));
            }
          }
          if (flag[0] == 'm' && parseMemFlag(flag)) {
            break;
          }
          if (argLength < 3) {
            char* message = _glom_strings(3, "\"", currentArg, 
                                          "\" is not a valid argument");
            _printError(message, 0, 0);
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

      case 'h':
        if (currentArg[2] == '\0') {
          printHelp = 1;
        } else {
          unexpectedArg(currentArg);
        }
        break;

      case 'n':
        if (currentArg[2] == 'l') {
          const char* numPtr;
          char numLocalesBuffer[128];
          if (currentArg[3] == '\0') {
            i++;
            if (i >= argc) {
              _printError("-nl flag is missing <numLocales> argument", 0, 0);
            }
            currentArg = argv[i];
            numPtr = currentArg;
          } else {
            numPtr = &(currentArg[3]);
          }
          parseNumLocales(numPtr);
          sprintf(numLocalesBuffer, "ChapelBase.numLocales=%d", _argNumLocales);
          addToConfigList(numLocalesBuffer, 1);
          break;
        }
        unexpectedArg(currentArg);
        break;

      case 's':
        {
          int isSingleArg = 1;
          if (argLength < 3) {
            char* message = _glom_strings(3, "\"", currentArg, 
                                          "\" is not a valid argument");
            _printError(message, 0, 0);
          }
          if (strncmp(currentArg+2, "numLocales", 10) == 0) {
            if (currentArg[12] == '=') {
              parseNumLocales(&(currentArg[13]));
            }
          }
          addToConfigList(currentArg + 2, isSingleArg);
          break;
        }

      case 'v':
        if (currentArg[2] == '\0') {
          verbosity = 2;
        } else {
          unexpectedArg(currentArg);
        }
        break;

      default:
        unexpectedArg(currentArg);
      }
    }
  }

  if (printHelp) {
    CreateConfigVarTable();    // get ready to start tracking config vars
    printHelpTable();
    printConfigVarTable();
  }
}
