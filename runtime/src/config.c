#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplio.h"
#include "config.h"

#define HASHSIZE 101

typedef struct _configVarType { /* table entry */
  char* varName;
  char* defaultValue;
  char* setValue;
 
  struct _configVarType* nextInList;
  struct _configVarType* nextInstalled;
} configVarType;


/* hash table */
static configVarType* configVarTable[HASHSIZE]; 
static configVarType* first = NULL;
static configVarType* last = NULL;

int initConfigVarTable(void) {
  int i;
  for (i = 0; i < HASHSIZE; i++) {
    configVarTable[i] = NULL;
  }
  return 0;
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
    fprintf(stdout, "%*s: ", longestName, configVar->varName);

    if (configVar->setValue) {
      fprintf(stdout, "%s (default:  %s)\n", configVar->setValue, 
	       configVar->defaultValue);
    } else {
      fprintf(stdout, "%s\n", configVar->defaultValue);
    }
  }
  exit(0);
}


static configVarType* lookupConfigVar(char* varName) {
  configVarType* configVar;
  unsigned hashValue;
  hashValue = hash(varName);

  /* This loops walks through the list of configuration variables 
     hashed to this location in the table. */
  for (configVar = configVarTable[hashValue]; 
       configVar != NULL; 
       configVar = configVar->nextInList) {
    if (strcmp(varName, configVar->varName) == 0) {
      return configVar;
    }
  }
  return NULL;
}


char* lookupSetValue(char* varName) {
  configVarType* configVar;
  configVar = lookupConfigVar(varName);
  if (configVar) {
    return configVar->setValue;
  } else {
    return NULL;
  }
}


int installConfigVar(char* varName, char* value, int isDefaultValue) {
  configVarType* configVar;
  unsigned hashValue;

  if (*varName == '\0') {
    fprintf(stderr, "***Error:  \"%s\" is not a valid variable name***\n", 
	    varName);
    exit(0);
  }
  configVar = lookupConfigVar(varName);
  if ((configVar == NULL) && !isDefaultValue) {
    fprintf(stderr, "***Error:  \"%s\" is not a config var***\n", varName);
    exit(0);
  }
  if (configVar && isDefaultValue) {
    fprintf(stderr, "***Error:  Program contains two config vars named \"%s\""
	    "***\n", varName);
    exit(0);
  }

  if (configVar == NULL) {
    configVar = (configVarType*) malloc(sizeof(configVarType));
    hashValue = hash(varName);
    configVar->nextInList = configVarTable[hashValue]; 
    configVarTable[hashValue] = configVar;

    if (first == NULL) {
      first = configVar;
    } else {
      last->nextInstalled = configVar;
    }
    last = configVar;
    configVar->varName = NULL;
    _copy_string(&configVar->varName, varName);
  } 

  if (isDefaultValue) {
    configVar->defaultValue = NULL;
    _copy_string(&configVar->defaultValue, value);
  } else {
    configVar->setValue = NULL;
    _copy_string(&configVar->setValue, value);
  }
  return 0;
}


int setInCommandLine_integer64(char* varName, _integer64* value) {
  int varSet = 0;
  char *setValue = lookupSetValue(varName);

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


int setInCommandLine_float64(char* varName, _float64* value) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName);

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


int setInCommandLine_boolean(char* varName, _boolean* value) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName);

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


int setInCommandLine_string(char* varName, _string* value) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName);

  if (setValue) {
    _copy_string(value, setValue);
    varSet = 1;
  }
  return varSet;
}


int setInCommandLine_complex128(char* varName, _complex128* value) {
  int varSet = 0;
  char* setValue = lookupSetValue(varName);

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
