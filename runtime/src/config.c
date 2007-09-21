#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplexit.h"
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

static configVarType _ambiguousConfigVar;
static configVarType* ambiguousConfigVar = &_ambiguousConfigVar;

static configVarType* lookupConfigVar(const char* varName, 
                                      const char* moduleName);


typedef struct _argType {
  configSource argSource;
  char* input;

  struct _argType* next;
} argType;



/* config var list */
static argType* firstArg = NULL;
static argType* lastArg = NULL;

void addToConfigList(const char* currentArg, configSource argSource) {
  char* description = _glom_strings(2, "argument list entry for ", currentArg);
  argType* arg = (argType*) _chpl_calloc(1, sizeof(argType), description, 0, 0);
  _chpl_free(description, 0, 0);

  arg->argSource = argSource;
  arg->input = string_copy(currentArg);
  
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


static void parseModVarName(char* modVarName, const char** moduleName, 
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
  char firstChar;
  char* value;
  char lastChar;
  const char* moduleName;
  char* varName;

  if (!equalsSign || !(equalsSign + 1)) {
    return 0;
  }

  firstChar = equalsSign[1];
  if ((firstChar != '"') && (firstChar != '\'')) {
    return 0;
  }

  value = equalsSign + 2;
  *equalsSign = '\0';
  lastChar = setConfigBuffer[stringLength - 1];

  parseModVarName(setConfigBuffer, &moduleName, &varName);
  
  if ((firstChar != lastChar) || (strlen(value) == 0)) {
    char nextChar = fgetc(argFile);
    do {
      switch (nextChar) {
      case EOF:
        {
          char* message;
          setConfigBuffer[stringLength] = '\0';
          message = _glom_strings(2, "Found end of file while reading string: ",
                                  equalsSign + 1);
          _printError(message, 0, 0);
          break;
        }
      case '\n':
        {
          char* message;
          setConfigBuffer[stringLength] = '\0';
          message = _glom_strings(2, "Found newline while reading string: ", 
                                  equalsSign + 1);
          _printError(message, 0, 0);
          break;
        }
      default:
        {
          if (stringLength >= _default_string_length - 1) {
            char dsl[1024];
            char* message;
            sprintf(dsl, "%d", _default_string_length);
            message = _glom_strings(2, "String exceeds the maximum "
                                          "string length of ", dsl);
            _printError(message, 0, 0);
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


static void parseSingleArg(char* currentArg, configSource argSource) {
  char* equalsSign = strchr(currentArg, '=');
  char* value = equalsSign + 1;
  const char* moduleName;
  char* varName;
  configVarType* configVar;

  if (equalsSign) {
    *equalsSign = '\0';
  }
  parseModVarName(currentArg, &moduleName, &varName);
  configVar = lookupConfigVar(varName, moduleName);
  if (configVar == NULL || configVar == ambiguousConfigVar) {
    const char* message;
    if (!equalsSign && argSource == ddash) {
      message = _glom_strings(3, "Unrecognized flag: '--", currentArg, "'");
    } else if (configVar == NULL) {
      if (strcmp(moduleName, "") != 0) {
        message = _glom_strings(5, "Module '", moduleName, 
                                "' has no configuration variable named '", 
                                varName, "'");
      } else {
        if (varName[0]) {
          message = _glom_strings(3, "Unrecognized configuration variable '",
                                  varName, "'");
        } else {
          if (argSource == ddash) {
            message = _glom_strings(3, "Unrecognized flag: '--", currentArg, "'");
          } else {
            message = "No configuration variable name specified";
          }
        }
      }
    } else {
      message = _glom_strings(5, "Configuration variable '", varName, 
                              "' is defined in more than one module.\n"
                              "       Use '--help' for a list of configuration variables\n"
                              "       and '-s<module>.", varName, "' to resolve the ambiguity.");
    }
    _printError(message, 0, 0);
  }

  if (equalsSign && *value) {
    initSetValue(varName, value, moduleName);
  } else {
    char* message;
    message = _glom_strings(3, "Configuration variable '", varName, 
                            "' is missing its initialization value");
    _printError(message, 0, 0);
  }
}


static void parseFileArgs(char* currentArg) {
  char* argFilename = currentArg;
  FILE* argFile = fopen(argFilename, "r");
  if (!argFile) {
    char* message = _glom_strings(2, "Unable to open ", argFilename);
    _printError(message, 0, 0);
  } 
  while (!feof(argFile)) {
    int numScans = 0;
    char setConfigBuffer[_default_string_length];
    numScans = fscanf(argFile, _default_format_read_string, setConfigBuffer);
    if (numScans == 1) {
      if (!aParsedString(argFile, setConfigBuffer)) {
        parseSingleArg(setConfigBuffer, fdash);
      } 
    }
  }
  fclose(argFile);
}


void parseConfigArgs(void) {
  argType* thisArg = firstArg;
  while (thisArg != NULL) {
    if (thisArg->argSource != fdash) {
      parseSingleArg(thisArg->input, thisArg->argSource);
    } else {
      parseFileArgs(thisArg->input);
    }
    thisArg = thisArg->next;
  }
}


void initConfigVarTable(void) {
  int i;
  for (i = 0; i < HASHSIZE; i++) {
    configVarTable[i] = NULL;
  }
}


/* hashing function */
static unsigned hash(const char* varName) {
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
  int foundUserConfigs = 0;
  int foundMultipleModules = 0;
  int thisName;

  for (configVar = firstInTable;
       configVar != NULL;
       configVar = configVar->nextInstalled) {

    if (strncmp(configVar->moduleName, "Chapel", 6) != 0) {
      if (foundUserConfigs == 0) {
        foundUserConfigs = 1;
        moduleName = configVar->moduleName;
      } else {
        if (strcmp(moduleName, configVar->moduleName) != 0) {
          foundMultipleModules = 1;
        }
      }
      thisName = strlen(configVar->varName);
      if (longestName < thisName)  {
        longestName = thisName;
      }
    }
  }

  moduleName = NULL;
  if (foundUserConfigs) {
    fprintf(stdout, "CONFIG VARS:\n");
    fprintf(stdout, "============\n");

    for (configVar = firstInTable; 
         configVar != NULL; 
         configVar = configVar->nextInstalled) {

      if (strncmp(configVar->moduleName, "Chapel", 6) != 0) {
        if (foundMultipleModules) {
          if (moduleName == NULL) {
            moduleName = configVar->moduleName;
            fprintf(stdout, "%s's config vars:\n", configVar->moduleName);
          }
          if (strcmp(configVar->moduleName, moduleName) != 0) {
            fprintf(stdout, "\n");
            fprintf(stdout, "%s's config vars:\n", configVar->moduleName);
            moduleName = configVar->moduleName;
          }
        }
        fprintf(stdout, "  %*s: ", longestName, configVar->varName);
        fprintf(stdout, "%s", configVar->defaultValue);
        if (configVar->setValue) {
          fprintf(stdout, " (configured to %s)", configVar->setValue);
        }
        fprintf(stdout, "\n");
      }
    }
  }
  _chpl_exit_any(0);
}


static configVarType* lookupConfigVar(const char* varName, 
                                      const char* moduleName) {
  configVarType* configVar = NULL;
  configVarType* foundConfigVar = NULL; 
  unsigned hashValue;
  int numTimesFound = 0;
  hashValue = hash(varName);

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


void initSetValue(char* varName, char* value, const char* moduleName) {
  configVarType* configVar;
  if  (*varName == '\0') {
    const char* message = "No variable name given";
    _printError(message, 0, 0);
  }
  configVar = lookupConfigVar(varName, moduleName);
  if (configVar == NULL || configVar == ambiguousConfigVar) {
    _printInternalError("unknown config var case not handled appropriately");
  }
  configVar->setValue = string_copy(value);
}


char* lookupSetValue(const char* varName, const char* moduleName) {
  configVarType* configVar;
  if (strcmp(moduleName, "") == 0) {
    const char* message = "Attempted to lookup value with the module name an "
      "empty string";
    _printInternalError(message);
  }

  configVar = lookupConfigVar(varName, moduleName);
  if (configVar) {
    return configVar->setValue;
  } else {
    return NULL;
  }
}


void installConfigVar(const char* varName, const char* value, 
                      const char* moduleName) {
  unsigned hashValue;
  char* description = _glom_strings(2, "config table entry for ", varName);
  configVarType* configVar = (configVarType*) 
    _chpl_calloc(1, sizeof(configVarType), description, 0, 0);
  _chpl_free(description, 0, 0);

  hashValue = hash(varName);
  configVar->nextInBucket = configVarTable[hashValue]; 
  configVarTable[hashValue] = configVar;
  if (firstInTable == NULL) {
    firstInTable = configVar;
  } else {
    lastInTable->nextInstalled = configVar;
  }
  lastInTable = configVar;
  configVar->varName = string_copy(varName);
  configVar->moduleName = string_copy(moduleName);
  configVar->defaultValue = string_copy(value);
} 
