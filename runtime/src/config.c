#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplio.h"
#include "chplmem.h"
#include "config.h"
#include "chplrt.h"

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
static configVarType* first = NULL;
static configVarType* last = NULL;

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

  if (first) {
    fprintf(stdout, "CONFIG VARS:\n");
    fprintf(stdout, "============\n");
  }

  for (configVar = first;
       configVar != NULL;
       configVar = configVar->nextInstalled) {

    int thisName = strlen(configVar->varName);
    if (longestName < thisName)  {
      longestName = thisName;
    }
  }

  for (configVar = first; 
       configVar != NULL; 
       configVar = configVar->nextInstalled) {

    if (moduleName == NULL) {
      moduleName = configVar->moduleName;
      if (strcmp(first->moduleName, last->moduleName) != 0) {
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
    fprintf(stderr, "***Error:  No variable name given***\n");
    exit(0);
  }
  configVarType* configVar = lookupConfigVar(varName, moduleName);
  if (configVar == NULL) {
    if (strcmp(moduleName, "") != 0) {
      fprintf(stderr, "***Error:  There is no \"%s\" config var in %s***\n", 
              varName, moduleName);
      exit(0);
    } else {
      fprintf(stderr, "***Error:  There is no config var \"%s\" in the "
              "program***\n", varName);
      exit(0);
    }
  } else if (configVar == ambiguousConfigVar) {
    fprintf(stderr, "***Error:  Config var \"%s\" is defined in more than one "
            "module.  Use \"-h\" for a list of config vars and \"-s<module>"
            ".%s\" to indicate which to use.***\n", varName, varName);
    exit(0);
  }
  _copy_string(&configVar->setValue, value);
}


char* lookupSetValue(char* varName, char* moduleName) {
  if (strcmp(moduleName, "") == 0) {
    fprintf(stderr, "***Internal Error:  Attempted to lookup value with "
            "the module name an empty string***\n");
    exit(0);
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
  if (first == NULL) {
    first = configVar;
  } else {
    last->nextInstalled = configVar;
  }
  last = configVar;
  _copy_string(&configVar->varName, varName);
  _copy_string(&configVar->moduleName, moduleName);
  _copy_string(&configVar->defaultValue, value);
} 


int setInCommandLine_integer64(char* varName, _integer64* value, 
                               char* moduleName) {
  int varSet = 0;
  char *setValue = lookupSetValue(varName, moduleName);

  if (setValue) {
    char extraChars;
    int numScans = sscanf(setValue, _default_format_read_integer64"%c", 
                      value, &extraChars);
    if (numScans == 1) {
      varSet = 1;
    } else {
      fprintf(stderr, "***Error:  \"%s\" is not a valid value for config var"
              " \"%s\" of type integer***\n", setValue, varName);
      exit(0);
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
      fprintf(stderr, "***Error:  \"%s\" is not a valid value for config var"
              " \"%s\" of type float***\n", setValue, varName);
      exit(0);
    }
  }
  return varSet;
}


int setInCommandLine_boolean(char* varName, _boolean* value, 
                             char* moduleName) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName, moduleName);

  if (setValue) {
    int validBoolean = _string_to_boolean(setValue, value);
    if (validBoolean) {
      varSet = 1;
    } else {
      fprintf(stderr, "***Error:  \"%s\" is not a valid value for config var"
              " \"%s\" of type boolean***\n", setValue, varName);
      exit(0);
    }
  }
  return varSet;
}


int setInCommandLine_string(char* varName, _string* value, char* moduleName) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName, moduleName);

  if (setValue) {
    _copy_string(value, setValue);
    varSet = 1;
  }
  return varSet;
}


int setInCommandLine_complex128(char* varName, _complex128* value, 
                                char* moduleName) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName, moduleName);

  if (setValue) {
    char extraChars;
    char imaginaryI = 'i';
    int numScans = sscanf(setValue, _default_format_read_complex128"%c", 
                      &(value->re), &(value->im), &imaginaryI, &extraChars);
    if ((numScans == 3) && (imaginaryI == 'i')) {
      varSet = 1;
    } else {
      fprintf(stderr, "***Error:  \"%s\" is not a valid value for config var "
              "\"%s\" of type complex***\n", setValue, varName); 
      exit(0);
    }
  }
  return varSet;
}
