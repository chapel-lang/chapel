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

  if ((*varName == '\0') || (*value == '\0')) {
      fprintf(stderr, "***Error:  \"-s%s=%s\" is not a valid argument***\n", 
	      varName, value);
      exit(0);
  }
  configVar = lookupConfigVar(varName);
  if (configVar == NULL) { /* not found */
    configVar = (configVarType*) malloc(sizeof(*configVar));
    hashValue = hash(varName);
    configVar->next   = configVarTable[hashValue]; 
    configVarTable[hashValue] = configVar;
    configVar->varName = varName;
  }
  configVar->value = value;
  return 0;
}


int setInCommandLine(char* varName, _integer64* value) {
  int varSet = 0;
  configVarType* configVar = NULL;
  
  configVar = lookupConfigVar(varName);
  if (configVar) {
    if (sscanf(configVar->value, _default_format_read_integer64, value) == 1) {
      varSet = 1;
    } else {
      fprintf(stderr, "***Error:  \"-s%s=%s\" is not a valid argument***\n", 
	      configVar->varName, configVar->value);
      exit(0);
    }
  }
  return varSet;
}

