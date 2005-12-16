#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplio.h"
#include "chplmem.h"
#include "chplrt.h"
#include "config.h"
#include "error.h"


#define HASHSIZE 101

typedef struct _configVarType { /* table entry */
  char* varName;
  char* moduleName;
  char* defaultValue;
  char* setValue;

  struct _configVarType* nextInBucket;
  struct _configVarType* nextInstalled;
} configVarType;


/* hash table */
static configVarType* configVarTable[HASHSIZE]; 
static configVarType* firstInTable = NULL;
static configVarType* lastInTable = NULL;


typedef struct _argType {
  int isSingleArg;
  char* input;

  struct _argType* next;
} argType;


/* config var list */
static argType* firstArg = NULL;
static argType* lastArg = NULL;

void addToConfigList(char* currentArg, int isSingleArg) {
  char* description = _glom_strings(2, "argument list entry for ", currentArg);
  argType* arg = (argType*) _chpl_calloc(1, sizeof(argType), description);
  _chpl_free(description);

  arg->isSingleArg = isSingleArg;
  _copy_string(arg->input, currentArg);
  
  if (firstArg == NULL) {
    firstArg = arg;
  } else {
    lastArg->next = arg;
  }
  lastArg = arg;
}


int askedToParseArgs(void) {
  int answer = 0;
  if (firstArg != NULL) {
    answer = 1;
  }
  return answer;
}


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
        {
          setConfigBuffer[stringLength] = '\0';
          char* message = _glom_strings(2, "Found end of file while reading "
                                        "string: ", equalsSign + 1);
          printError(message);
          break;
        }
      case '\n':
        {
          setConfigBuffer[stringLength] = '\0';
          char* message = _glom_strings(2, "Found newline while reading "
                                        "string: ", equalsSign + 1);
          printError(message);
          break;
        }
      default:
        {
          if (stringLength >= _default_string_length - 1) {
            char dsl[1024];
            sprintf(dsl, "%d", _default_string_length);
            char* message = _glom_strings(2, "String exceeds the maximum "
                                          "string length of ", dsl);
            printError(message);
          }
          setConfigBuffer[stringLength] = nextChar;
          stringLength++;
          nextChar = fgetc(argFile);
        }
      }
    } while (nextChar != firstChar);
  } else {
    stringLength--;
  }
  setConfigBuffer[stringLength] = '\0';
  initSetValue(varName, value, moduleName);
  return 1;
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
        char* message = _glom_strings(3, "Configuration variable \"", 
                                      varName, "\" is missing its "
                                      "initialization value");
        printError(message);
      }
      initSetValue(varName, value, moduleName);
    } else {
      char* message = _glom_strings(3, "\"", value, "\" is not a valid value");
      printError(message);
    }
  } else {
    char* message = _glom_strings(3, "\"", currentArg, "\" is not a valid "
                                  "argument");
    printError(message);
  }
}


static void parseFileArgs(char* currentArg) {
  char* argFilename = currentArg;
  FILE* argFile = fopen(argFilename, "r");
  if (!argFile) {
    char* message = _glom_strings(2, "Unable to open ", argFilename);
    printError(message);
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
}


void parseConfigArgs(void) {
  argType* thisArg = firstArg;
  while (thisArg != NULL) {
    if (thisArg->isSingleArg) {
      parseSingleArg(thisArg->input);
    } else {
      parseFileArgs(thisArg->input);
    }
    thisArg = thisArg->next;
  }
}


static int printHelp = 0; 

void printHelpMessage(void) {
  printHelp = 1;
}


int askedToPrintHelpMessage(void) {
  return printHelp;
}


void initConfigVarTable(void) {
  int i;
  for (i = 0; i < HASHSIZE; i++) {
    configVarTable[i] = NULL;
  }
}


/* hashing function */
static unsigned hash(char* varName) {
  unsigned hashValue;
  for (hashValue = 0; *varName != '\0'; varName++) {
    hashValue = *varName + 31 * hashValue;
  }
  return hashValue % HASHSIZE;
}


void printConfigVarTable(void) {
  configVarType* configVar = NULL; 
  int longestName = 0;
  char* moduleName = NULL;

  if (firstInTable) {
    fprintf(stdout, "CONFIG VARS:\n");
    fprintf(stdout, "============\n");
  }

  for (configVar = firstInTable;
       configVar != NULL;
       configVar = configVar->nextInstalled) {

    int thisName = strlen(configVar->varName);
    if (longestName < thisName)  {
      longestName = thisName;
    }
  }

  for (configVar = firstInTable; 
       configVar != NULL; 
       configVar = configVar->nextInstalled) {

    if (moduleName == NULL) {
      moduleName = configVar->moduleName;
      if (strcmp(firstInTable->moduleName, lastInTable->moduleName) != 0) {
        fprintf(stdout, "%s's config vars:\n", configVar->moduleName);
      }
    }
    if (strcmp(configVar->moduleName, moduleName) != 0) {
      fprintf(stdout, "\n");
      fprintf(stdout, "%s's config vars:\n", configVar->moduleName);
      moduleName = configVar->moduleName;
    }
    fprintf(stdout, "  %*s: ", longestName, configVar->varName);
    if (configVar->setValue) {
      fprintf(stdout, "%s (default:  %s)\n", configVar->setValue, 
               configVar->defaultValue);
    } else {
      fprintf(stdout, "%s\n", configVar->defaultValue);
    }
  }
  exit(0);
}


static configVarType _ambiguousConfigVar;
static configVarType* ambiguousConfigVar = &_ambiguousConfigVar;

static configVarType* lookupConfigVar(char* varName, char* moduleName) {
  configVarType* configVar = NULL;
  configVarType* foundConfigVar = NULL; 
  unsigned hashValue;
  hashValue = hash(varName);
  int numTimesFound = 0;

  /* This loops walks through the list of configuration variables 
     hashed to this location in the table. */
  for (configVar = configVarTable[hashValue]; 
       configVar != NULL; 
       configVar = configVar->nextInBucket) {

    if (strcmp(configVar->varName, varName) == 0) {
      if (strcmp(moduleName, "") == 0) {
        numTimesFound++;
        if (numTimesFound == 1) {
          foundConfigVar = configVar;
        } else {
          foundConfigVar = ambiguousConfigVar;
        }
      } else {
        if (strcmp(configVar->moduleName, moduleName) == 0) {
          foundConfigVar = configVar;
        }
      }
    }
  }
  return foundConfigVar;
}


void initSetValue(char* varName, char* value, char* moduleName) {
  if  (*varName == '\0') {
    char* message = "No variable name given";
    printError(message);
  }
  configVarType* configVar = lookupConfigVar(varName, moduleName);
  if (configVar == NULL) {
    if (strcmp(moduleName, "") != 0) {
      char* message = _glom_strings(4, "There is no \"", varName, "\" config "
                                    "var in ", moduleName);
      printError(message);
    } else {
      char* message = _glom_strings(3, "There is no config var \"", varName, 
                                    "\" in the program");
      printError(message);
    }
  } else if (configVar == ambiguousConfigVar) {
    char* message = _glom_strings(5, "Config var \"", varName, "\" is defined "
                                  "in more than one module.  Use \"-h\" for "
                                  "a list of config vars and \"-s<module>.", 
                                  varName, "\" to indicate which to use.");
    printError(message);
  }
  _copy_string(configVar->setValue, value);
}


char* lookupSetValue(char* varName, char* moduleName) {
  if (strcmp(moduleName, "") == 0) {
    char* message = "Attempted to lookup value with the module name an "
      "empty string";
    printInternalError(message);
  }

  configVarType* configVar;
  configVar = lookupConfigVar(varName, moduleName);
  if (configVar) {
    return configVar->setValue;
  } else {
    return NULL;
  }
}


void installConfigVar(char* varName, char* value, char* moduleName) {
  unsigned hashValue;
  char* description = _glom_strings(2, "config table entry for ", varName);
  configVarType* configVar = (configVarType*) 
    _chpl_calloc(1, sizeof(configVarType), description);
  _chpl_free(description);

  hashValue = hash(varName);
  configVar->nextInBucket = configVarTable[hashValue]; 
  configVarTable[hashValue] = configVar;
  if (firstInTable == NULL) {
    firstInTable = configVar;
  } else {
    lastInTable->nextInstalled = configVar;
  }
  lastInTable = configVar;
  _copy_string(configVar->varName, varName);
  _copy_string(configVar->moduleName, moduleName);
  _copy_string(configVar->defaultValue, value);
} 


int setInCommandLine_integer64(char* varName, _integer64* value, 
                               char* moduleName) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName, moduleName);

  if (setValue) {
    char extraChars;
    int numScans = sscanf(setValue, _default_format_read_integer64"%c", 
                      value, &extraChars);
    if (numScans == 1) {
      varSet = 1;
    } else {
      char* message = _glom_strings(5, "\"", setValue, "\" is not a valid "
                                    "value for config var \"", varName, 
                                    "\" of type integer");
      printError(message);
    }
  }   
  return varSet;
}


int setInCommandLine_float64(char* varName, _float64* value, 
                             char* moduleName) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName, moduleName);

  if (setValue) {
    char extraChars;
    int numScans = sscanf(setValue, _default_format_read_float64"%c", 
                      value, &extraChars);
    if (numScans == 1) {
      varSet = 1;
    } else {
      char* message = _glom_strings(5, "\"", setValue, "\" is not a valid "
                                    "value for config var \"", varName, 
                                    "\" of type float");
      printError(message);
    }
  }
  return varSet;
}


int setInCommandLine_boolean(char* varName, _boolean* value, 
                             char* moduleName) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName, moduleName);

  if (setValue) {
    int validBoolean = 0;
    if (setValue) {
      if (strcmp(setValue, "true") == 0) {
        *value = true;
        validBoolean = 1;
      } else if (strcmp(setValue, "false") == 0) {
        *value = false;
        validBoolean = 1;
      }
    }

    if (validBoolean) {
      varSet = 1;
    } else {
      char* message = _glom_strings(5, "\"", setValue, "\" is not a valid "
                                    "value for config var \"", varName, 
                                    "\" of type boolean");
      printError(message);
    }
  }
  return varSet;
}


int setInCommandLine_string(char* varName, _string* value, char* moduleName) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName, moduleName);

  if (setValue) {
    _copy_string(*value, setValue);
    varSet = 1;
  }
  return varSet;
}


int setInCommandLine_chpl_complex(char* varName, _complex128** value, 
                                  char* moduleName) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName, moduleName);

  if (setValue) {
    char extraChars;
    char imaginaryI = 'i';
    int numScans;
    *value = (_complex128*)_chpl_malloc(1, sizeof(_complex128), "config complex");
    numScans = sscanf(setValue, _default_format_read_complex128"%c", 
                      &((*value)->re), &((*value)->im), &imaginaryI, &extraChars);
    if ((numScans == 3) && (imaginaryI == 'i')) {
      varSet = 1;
    } else {
      char* message = _glom_strings(5, "\"", setValue, "\" is not a valid "
                                    "value for config var \"", varName, 
                                    "\" of type complex");
      printError(message);
    }
  }
  return varSet;
}
