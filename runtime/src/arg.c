#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"
#include "chplio.h"
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


int parseArgs(int argc, char* argv[]) {
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
      case 's':
        if (argLength < 3) {
          fprintf(stderr, "***Error:  \"%s\" is not a valid argument***\n",
                  currentArg);
          exit(0);
        }
        parseSingleArg(currentArg + 2);
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
          break;
        }
      case 'h':
        printConfigVarTable();
        break;
      default:
        fprintf(stderr, "***Error: unexpected flag: '%s'\n", currentArg);
        exit(0);
      }
    }
  }
  return 0;
}
