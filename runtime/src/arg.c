#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"
#include "chplio.h"
#include "chplmem.h"
#include "config.h"
#include "chplrt.h"


static void parseModVarName(char* modVarName, char** moduleName, 
                            char** varName) {
  char* dot = strrchr(modVarName, '.');
  if (dot) {
    *dot = '\0';
    *moduleName = modVarName;
    *varName = dot + 1;
  } else {
    *moduleName = "";
    *varName = modVarName;
  }
}


static void parseSingleArg(char* currentArg) {
  char* equalsSign = strchr(currentArg, '=');     

  if (equalsSign) {
    *equalsSign = '\0';
    char* value = equalsSign + 1;
    
    if (value) {
      char* moduleName;
      char* varName;
      parseModVarName(currentArg, &moduleName, &varName);

      if (*value == '\0') {
        fprintf(stderr, "***Error:  Configuration variable \"%s\" is missing"
                " its initialization value***\n", varName);
        exit(0);
      }
      initSetValue(varName, value, moduleName);
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


/* This function parses a config var of type string, and sets its value in 
   the hash table.  
*/
static int aParsedString(FILE* argFile, char* setConfigBuffer) {
  char* equalsSign = strchr(setConfigBuffer, '=');
  int stringLength = strlen(setConfigBuffer);

  if (!equalsSign || !(equalsSign + 1)) {
    return 0;
  }
  char firstChar = equalsSign[1];
  if ((firstChar != '"') && (firstChar != '\'')) {
    return 0;
  }

  char* value = equalsSign + 2;
  *equalsSign = '\0';
  char lastChar = setConfigBuffer[stringLength - 1];

  char* moduleName;
  char* varName;
  parseModVarName(setConfigBuffer, &moduleName, &varName);
  
  if ((firstChar != lastChar) || (strlen(value) == 0)) {
    char nextChar = fgetc(argFile);
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
  initSetValue(varName, value, moduleName);
  return 1;
}


static _integer64 getIntArg(char* valueString, char* memFlag) {
  char extraChars;
  _integer64 value;

  int numScans = sscanf(valueString, _default_format_read_integer64"%c", 
                        &value, &extraChars);
  if (numScans != 1) {
    fprintf(stderr, "***Error:  The \"%s\" flag is missing its byte value"
            "***\n", memFlag);
    exit(0);
  }
  return value; 
}


static char* getStringArg(char* valueString, char* memFlag) {
  if (!valueString) {
    fprintf(stderr, "***Error:  The \"%s\" flag is missing its option***"
            "\n", memFlag);
    exit(0);
  }
  return valueString;
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
    fprintf(stderr, "***Error:  \"%s\" is not a valid execution option***\n",
            memFlag);
  }
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


void printHelpTable(void) {
  typedef struct _flagType {
    char* flag;
    char* description;
    char headerType;
  } flagType;

  static flagType flagList[] = {
    {"-h", "print this message", 'g'},

    {"-s<configVar>=<value>", "set the value of a config var", 'c'},    
    {"-f<filename>", "read in a file of config var assignments", 'c'},

    {"--memmax=<n>", "simulate 'n' bytes of memory available", 'm'}, 
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


void parseArgs(int argc, char* argv[]) {
  int i;
  
  for (i = 1; i < argc; i++) {
    int argLength = 0;
    char* currentArg = argv[i];
    argLength = strlen(currentArg);

    if (argLength < 2) {
      fprintf(stderr, "***Error:  \"%s\" is not a valid argument***\n", 
              currentArg);
      exit(0);
    }
    
    switch (currentArg[0]) {
    case '-':
      switch (currentArg[1]) {
      case '-':
        parseMemFlag(currentArg + 2);
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
              if (!aParsedString(argFile, setConfigBuffer)) {
                parseSingleArg(setConfigBuffer);
              } 
            }
          }
          fclose(argFile);
          break;
        }

      case 's':
        if (argLength < 3) {
          fprintf(stderr, "***Error:  \"%s\" is not a valid argument***\n",
                  currentArg);
          exit(0);
        }
        parseSingleArg(currentArg + 2);
        break;

      case 'h':
        printHelpTable();
        printConfigVarTable();
        break;

      default:
        fprintf(stderr, "***Error: unexpected flag: '%s'\n", currentArg);
        exit(0);
      }
    }
  }
}
