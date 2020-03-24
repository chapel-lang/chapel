/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chplrt.h"

#include "arg.h"
#include "chplcgfns.h"
#include "chplexit.h"
#include "chplio.h"
#include "chpl-mem.h"
#include "chpl-linefile-support.h"
#include "config.h"
#include "error.h"

#include <stdio.h>
#include <string.h>


#define HASHSIZE 101

typedef struct _configVarType { /* table entry */
  char* varName;
  const char* moduleName;
  char* defaultValue;
  char* setValue;
  int private;

  struct _configVarType* nextInBucket;
  struct _configVarType* nextInstalled;
} configVarType;


/* hash table */
static configVarType* configVarTable[HASHSIZE];
static configVarType* firstInTable = NULL;
static configVarType* lastInTable = NULL;

static configVarType _ambiguousConfigVar;
static configVarType* ambiguousConfigVar = &_ambiguousConfigVar;

static configVarType* lookupConfigVar(const char* moduleName,
                                      const char* varName);


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
static int aParsedString(FILE* argFile, char* setConfigBuffer,
                         int32_t lineno, int32_t filename) {
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
    int nextChar = fgetc(argFile);
    do {
      switch (nextChar) {
      case EOF:
        {
          char* message;
          setConfigBuffer[stringLength] = '\0';
          message = chpl_glom_strings(2, "Found end of file while reading string: ",
                                      equalsSign + 1);
          chpl_error(message, lineno, filename);
          break;
        }
      case '\n':
        {
          char* message;
          setConfigBuffer[stringLength] = '\0';
          message = chpl_glom_strings(2, "Found newline while reading string: ",
                                      equalsSign + 1);
          chpl_error(message, lineno, filename);
          break;
        }
      default:
        {
          if (stringLength >= _default_string_length - 1) {
            char dsl[1024];
            char* message;
            sprintf(dsl, "%d", _default_string_length);
            message = chpl_glom_strings(2, "String exceeds the maximum string length of ",
                                        dsl);
            chpl_error(message, lineno, filename);
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
  initSetValue(varName, value, moduleName, lineno, filename);
  return 1;
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
  const char* moduleName = NULL;
  int foundUserConfigs = 0;
  int foundMultipleModules = 0;
  int thisName;

  for (configVar = firstInTable;
       configVar != NULL;
       configVar = configVar->nextInstalled) {

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

  moduleName = NULL;
  if (foundUserConfigs) {
    fprintf(stdout, "CONFIG VARS:\n");
    fprintf(stdout, "============\n");

    for (configVar = firstInTable;
         configVar != NULL;
         configVar = configVar->nextInstalled) {

        if (foundMultipleModules) {
          if (moduleName == NULL) {
            moduleName = configVar->moduleName;
            fprintf(stdout, "%s config vars:\n", configVar->moduleName);
          }
          if (strcmp(configVar->moduleName, moduleName) != 0) {
            fprintf(stdout, "\n");
            fprintf(stdout, "%s config vars:\n", configVar->moduleName);
            moduleName = configVar->moduleName;
          }
        }
        fprintf(stdout, "  %*s: ", longestName, configVar->varName);
        fprintf(stdout, "%s", configVar->defaultValue);
        if (configVar->setValue || configVar->private) {
          fprintf(stdout, " (");
          if (configVar->private) {
            fprintf(stdout, "private");
            if (configVar->setValue) {
              fprintf(stdout, ", ");
            }
          }
          if (configVar->setValue) {
            fprintf(stdout, "configured to %s", configVar->setValue);
          }
          fprintf(stdout, ")");
        }
        fprintf(stdout, "\n");
      }
  }

}


static configVarType* lookupConfigVar(const char* moduleName,
                                      const char* varName) {
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
        // only public configs can be referred to in an unqualified manner
        if (!configVar->private) {
          numTimesFound++;
          if (numTimesFound == 1) {
            foundConfigVar = configVar;
          } else {
            foundConfigVar = ambiguousConfigVar;
          }
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


void initSetValue(const char* varName, const char* value,
                  const char* moduleName,
                  int32_t lineno, int32_t filename) {
  configVarType* configVar;
  if  (*varName == '\0') {
    const char* message = "No variable name given";
    chpl_error(message, lineno, filename);
  }
  configVar = lookupConfigVar(moduleName, varName);
  if (configVar == NULL) {
    const char* configName = ((moduleName == NULL || !strcmp(moduleName, ""))
                              ? varName
                              : chpl_glom_strings(3,
                                                  moduleName, ".", varName));
    const char* message = chpl_glom_strings(3, "attempt to set config named '",
                                            configName,
                                            "', but there is no such config");
    chpl_error(message, lineno, filename);
  } else if (configVar == ambiguousConfigVar) {
    const char* message = chpl_glom_strings(5,
                                            "Multiple modules define a config "
                                            "named '", varName,
                                            "'.  Use '--help' for a list and "
                                            "disambiguate using '<moduleName>.",
                                            varName, "'.");
    chpl_error(message, lineno, filename);
  }
  if (strcmp(varName, "numLocales") == 0) {
    parseNumLocales(value, lineno, filename);
  }
  configVar->setValue = chpl_glom_strings(1, value);
}



const char* lookupSetValue(const char* varName, const char* moduleName) {
  configVarType* configVar;
  if (strcmp(moduleName, "") == 0) {
    const char* message = "Attempted to lookup value with the module name an "
      "empty string";
    chpl_internal_error(message);
  }

  configVar = lookupConfigVar(moduleName, varName);
  if (configVar) {
    return configVar->setValue;
  } else {
    return NULL;
  }
}


void installConfigVar(const char* varName, const char* value,
                      const char* moduleName, int private) {
  unsigned hashValue;
  configVarType* configVar = (configVarType*)
    chpl_mem_allocMany(1, sizeof(configVarType), CHPL_RT_MD_CF_TABLE_DATA, 0, 0);

  hashValue = hash(varName);
  configVar->nextInBucket = configVarTable[hashValue];
  configVar->nextInstalled = NULL;
  configVarTable[hashValue] = configVar;
  if (firstInTable == NULL) {
    firstInTable = configVar;
  } else {
    lastInTable->nextInstalled = configVar;
  }
  lastInTable = configVar;
  configVar->varName = chpl_glom_strings(1, varName);
  configVar->moduleName = chpl_glom_strings(1, moduleName);
  configVar->defaultValue = chpl_glom_strings(1, value);
  configVar->setValue = NULL;
  configVar->private = private;
}


static configVarType* breakIntoPiecesAndLookup(char* str, char** equalsSign,
                                               const char** moduleName,
                                               char** varName,
                                               int32_t lineno,
                                               int32_t filename) {
  configVarType* configVar;

  *equalsSign = strchr(str, '=');
  if (*equalsSign) {
    **equalsSign = '\0';
  }
  parseModVarName(str, moduleName, varName);
  configVar = lookupConfigVar(*moduleName, *varName);
  if (configVar == ambiguousConfigVar) {
    const char* message = chpl_glom_strings(5, "Configuration variable '",
                                            *varName,
                                            "' is defined in more than one "
                                            "module.  Use '--help' for a list "
                                            "of configuration variables and "
                                            "'-s<module>.",
                                            *varName, "' to disambiguate.");
    chpl_error(message, lineno, filename);
  }
  return configVar;
}


static void handleUnexpectedConfigVar(const char* moduleName, char* varName,
                                      int32_t lineno, int32_t filename) {
  const char* message;
  if (moduleName[0]) {
    message = chpl_glom_strings(5, "Module '", moduleName,
                                "' has no configuration variable named '",
                                varName, "'");
  } else if (varName[0]) {
    message = chpl_glom_strings(3, "Unrecognized configuration variable '",
                                varName, "'");
  } else {
    message = "No configuration variable name specified";
  }
  chpl_error(message, lineno, filename);
}


int handlePossibleConfigVar(int* argc, char* argv[], int argnum,
                            int32_t lineno, int32_t filename) {
  int retval = 0;
  int arglen = strlen(argv[argnum]+2)+1;
  char* argCopy = chpl_mem_allocMany(arglen, sizeof(char),
                                     CHPL_RT_MD_CFG_ARG_COPY_DATA, argnum,
                                     CHPL_FILE_IDX_COMMAND_LINE);
  char* equalsSign;
  const char* moduleName;
  char* varName;
  configVarType* configVar;

  strcpy(argCopy, argv[argnum]+2);
  configVar = breakIntoPiecesAndLookup(argCopy, &equalsSign, &moduleName,
                                       &varName, lineno, filename);
  if (configVar == NULL) {
    if (argv[argnum][1] == '-') { // this is a -- argument
      retval = handleNonstandardArg(argc, argv, argnum, lineno, filename);
    } else {                      // this is a -s argument
      handleUnexpectedConfigVar(moduleName, varName, lineno, filename);
    }
  } else {
    char* value = equalsSign + 1;
    if (equalsSign && *value) {
      initSetValue(varName, value, moduleName, lineno, filename);
    } else if (!strcmp(configVar->defaultValue, "bool")) {
      initSetValue(varName, "true", moduleName, lineno, filename);
    } else {
      if (argnum + 1 >= *argc) {
        char* message = chpl_glom_strings(3, "Configuration variable '", varName,
                                          "' is missing its initialization value");
        chpl_error(message, lineno, filename);
      } else {
        initSetValue(varName, argv[argnum+1], moduleName, lineno, filename);
        retval = 1;
      }
    }
  }

  chpl_mem_free(argCopy, argnum, CHPL_FILE_IDX_COMMAND_LINE);
  return retval;
}

// TODO: Change all the 0 linenos below into real line numbers
void parseConfigFile(const char* configFilename,
                     int32_t lineno, int32_t filename) {
  FILE* argFile = fopen(configFilename, "r");
  if (!argFile) {
    char* message = chpl_glom_strings(2, "Unable to open ", configFilename);
    chpl_error(message, lineno, filename);
  }
  chpl_saveFilename(configFilename); // CHPL_FILE_IDX_SAVED_FILENAME will now
                                     // give us configFilename
  while (!feof(argFile)) {
    int numScans = 0;
    char setConfigBuffer[_default_string_length];
    numScans = fscanf(argFile, _default_format_read_string, setConfigBuffer);
    if (numScans == 1) {
     if (*setConfigBuffer == '#') {
        if (!feof(argFile)) {
          do {
            char ch = fgetc(argFile);
            if (ch == '\n')
              break;
          } while (!feof(argFile));
        }
      } else if (!aParsedString(argFile, setConfigBuffer, 0,
                                CHPL_FILE_IDX_SAVED_FILENAME)) {
        char* equalsSign;
        const char* moduleName;
        char* varName;
        configVarType *configVar =
            breakIntoPiecesAndLookup(setConfigBuffer, &equalsSign, &moduleName,
                                     &varName, 0, CHPL_FILE_IDX_SAVED_FILENAME);
        if (configVar == NULL) {
          handleUnexpectedConfigVar(moduleName, varName, 0,
                                    CHPL_FILE_IDX_SAVED_FILENAME);
        } else {
          char* value = equalsSign + 1;
          if (equalsSign && *value) {
            initSetValue(varName, value, moduleName, 0,
                         CHPL_FILE_IDX_SAVED_FILENAME);
          } else {
            char configValBuffer[_default_string_length];
            numScans =
                fscanf(argFile, _default_format_read_string, configValBuffer);
            if (numScans != 1) {
              char *message =
                  chpl_glom_strings(3, "Configuration variable '", varName,
                                    "' is missing its initialization value");
              chpl_error(message, 0, CHPL_FILE_IDX_SAVED_FILENAME);
            } else {
              initSetValue(varName, configValBuffer, moduleName, 0,
                           CHPL_FILE_IDX_SAVED_FILENAME);
            }
          }
        }
      }
    }
  }
  fclose(argFile);
}


chpl_bool chpl_config_has_value(c_string v, c_string m) {
  return lookupSetValue(v, m) != NULL;
}


// Returning a c_string here is thread-safe because config consts
// are constant for the duration of the program.
c_string chpl_config_get_value(c_string v, c_string m) {
  return lookupSetValue(v, m);
}


