#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplio.h"
#include "config.h"

#define HASHSIZE 101

typedef struct _configVarType { /* table entry */
  char* varName;
  char* value;

  struct _configVarType* next;
} configVarType;


/* hash table */
static configVarType* configVarTable[HASHSIZE]; 

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


static configVarType* lookupConfigVar(char* varName) {
  configVarType* configVar;
  unsigned hashValue;
  hashValue = hash(varName);

  /* This loops walks through the list of configuration variables 
     hashed to this location in the table. */
  for (configVar = configVarTable[hashValue]; 
       configVar != NULL; 
       configVar = configVar->next) {
    if (strcmp(varName, configVar->varName) == 0) {
      return configVar;
    }
  }
  return NULL;
}


int installConfigVar(char* varName, char* value) {
  configVarType* configVar;
  unsigned hashValue;

  if (*varName == '\0') {
    fprintf(stderr, "***Error:  \"%s\" is not a valid variable name***\n", 
	    varName);
    exit(0);
  }
  if (*value == '\0') {
    fprintf(stderr, "***Error:  Configuration variable \"%s\" is missing its"
	    " initialization value***\n", varName);
      exit(0);
  }
  configVar = lookupConfigVar(varName);
  if (configVar == NULL) { /* not found */
    configVar = (configVarType*) malloc(sizeof(configVarType));
    hashValue = hash(varName);
    configVar->next = configVarTable[hashValue]; 
    configVarTable[hashValue] = configVar;
    configVar->varName = varName;
  }
  configVar->value = value;
  return 0;
}


int setInCommandLine_integer64(char* varName, _integer64* value) {
  int varSet = 0;
  int numScans = 0;
  configVarType* configVar = NULL;
  char extraChars;
  
  configVar = lookupConfigVar(varName);
  if (configVar) {
    numScans = sscanf(configVar->value, _default_format_read_integer64"%c", 
		      value, &extraChars);
    if (numScans == 1) {
      varSet = 1;
    } else {
      fprintf(stderr, "***Error:  \"%s\" is not a valid value for config var"
	      " \"%s\" of type integer***\n", configVar->value, 
	      configVar->varName);
      exit(0);
    }
  }
  return varSet;
}


int setInCommandLine_float64(char* varName, _float64* value) {
  int varSet = 0;
  int numScans = 0;
  configVarType* configVar = NULL;
  char extraChars;
  
  configVar = lookupConfigVar(varName);
  if (configVar) {
    numScans = sscanf(configVar->value, _default_format_read_float64"%c", 
		      value, &extraChars);
    if (numScans == 1) {
      varSet = 1;
    } else {
      fprintf(stderr, "***Error:  \"%s\" is not a valid value for config var"
	      " \"%s\" of type float***\n", configVar->value, 
	      configVar->varName);
      exit(0);
    }
  }
  return varSet;
}


int setInCommandLine_boolean(char* varName, _boolean* value) {
  int varSet = 0;
  int validBoolean = 0;
  configVarType* configVar = NULL;

  configVar = lookupConfigVar(varName);
  if (configVar) {
    validBoolean = _string_to_boolean(configVar->value, value);

    if (validBoolean) {
      varSet = 1;
    } else {
      fprintf(stderr, "***Error:  \"%s\" is not a valid value for config var"
	      " \"%s\" of type boolean***\n", configVar->value, 
	      configVar->varName);
      exit(0);
    }
  }
  return varSet;
}


int setInCommandLine_string(char* varName, _string* value) {
  int varSet = 0;
  configVarType* configVar = NULL;
  
  configVar = lookupConfigVar(varName);
  if (configVar) {
    _copy_string(value, configVar->value);
    varSet = 1;
  }
  return varSet;
}

int setInCommandLine_complex128(char* varName, _complex128* value) {
  int varSet = 0;
  char imaginaryI = 'i';
  int numScans;
  configVarType* configVar = NULL;
  char extraChars;

  configVar = lookupConfigVar(varName);
  if (configVar) {
    numScans = sscanf(configVar->value, _default_format_read_complex128"%c", 
		      &(value->re), &(value->im), &imaginaryI, &extraChars);
    if ((numScans == 3) && (imaginaryI == 'i')) {
      varSet = 1;
    } else {
      fprintf(stderr, "***Error:  \"%s\" is not a valid value for config var "
	      "\"%s\" of type complex***\n", configVar->value, 
	      configVar->varName); 
      exit(0);
    }
  }
  return varSet;
}
