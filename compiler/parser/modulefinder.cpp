/*
 * Copyright 2004-2014 Cray Inc.
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "modulefinder.tab.h"
#include "modulefinder.h"

#ifdef __cplusplus
extern "C" {
#endif
  extern FILE *yyin;
  /*
  extern char* yytext;
  extern char* yyfilename;
  extern int yylineno;
  */  
  int yyparse(void);
  /*
  void yyerror(const char* str);
  int yywrap(void);
  int getNextYYChar();
  */

#ifdef __cplusplus
}
#endif

int yylineno;
int yystartlineno;
char* yyfilename;

int depth;
int filemodule;

const int MAX_MOD_DEPTH = 256;

class ModInfo;

extern ModInfo* topLevelMod;

ModInfo* currentMods[MAX_MOD_DEPTH];

class ModInfo {
 public:
  char* name;
  ModInfo* sibling;
  ModInfo* child;

  ModInfo(void) {
    name = NULL;
    sibling = NULL;
    child = NULL;
  }

  ModInfo(int depth) {
    name = NULL;
    sibling = NULL;
    child = NULL;
    if (depth == 0) {
      topLevelMod->addChild(this);
    } else {
      if (depth == MAX_MOD_DEPTH) {
        fprintf(stderr, "ERROR: modules exceed maximum depth\n");
        exit(1);
      }
      currentMods[depth-1]->addChild(this);
    }
  }

  void addChild(ModInfo* newMod) {
    if (child == NULL) {
      child = newMod;
    } else {
      ModInfo* lastchild = child;
      while (lastchild->sibling != NULL) {
        lastchild = lastchild->sibling;
      }
      lastchild->sibling = newMod;
    }
  }

  void printAll() {
    if (name != NULL) {
      if (topLevelMod->name != NULL && this != topLevelMod) {
        fprintf(stderr, "%s.", topLevelMod->name);
      }
      fprintf(stderr, "%s\n", name);
    }
    if (sibling != NULL) {
      sibling->printAll();
    }
    if (child != NULL) {
      child->printAll();
    }
  }

};

ModInfo* topLevelMod;

void registerModule(char* newModName, int depth) {
  int i;

  //  fprintf(stderr, "Found module %s at depth %d\n", newModName, depth);

  ModInfo* newMod = new ModInfo(depth);
  currentMods[depth] = newMod;
  int nameLen = strlen(newModName) + 1;
  for (i=0; i<depth; i++) {
    nameLen += strlen(currentMods[i]->name) + 1;
  }
  newMod->name = new char[nameLen];
  newMod->name[0] = '\0';
  for (i=0; i<depth; i++) {
    strcat(newMod->name, currentMods[i]->name);
    strcat(newMod->name, ".");
  }
  strcat(newMod->name, newModName);
}


void registerTopLevelCode(void) {
  //  fprintf(stderr, "Found top-level code\n");
  int length = strlen(yyfilename) + 1;
  char* buff = new char[length];
  strcpy(buff, yyfilename);
  char* lastdot = strrchr(buff, '.');
  if (lastdot != NULL) {
    *lastdot = '\0';
  }
  char* lastslash = strrchr(buff, '/');
  if (lastslash != NULL) {
    buff = lastslash + 1;
  }
  //  fprintf(stderr, "buff = %s\n", buff);
  topLevelMod->name = new char[strlen(buff) + 1];
  strcpy(topLevelMod->name, buff);
}


void reset(void) {
  topLevelMod = new ModInfo();
  depth = 0;
  filemodule = 0;
}

void printResults(void) {
  fprintf(stderr, "%s:\n", yyfilename);
  topLevelMod->printAll();
  fprintf(stderr, "\n");
}


int main(int argc, char* argv[]) {
  int i;
  for (i=1; i<argc; i++) {
    reset();
    yyfilename = argv[i];
    yyin = fopen(yyfilename, "r");
    if (yyin) {
      yyparse();
    } else {
      fprintf(stderr, "ERROR: Cannot open %s\n", yyfilename);
      exit(1);
    }
    printResults();
  }
}
