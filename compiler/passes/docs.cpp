/*
 * Copyright 2004-2015 Cray Inc.
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

#include <map>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sys/stat.h>
#include <sys/types.h>

#include "driver.h"
#include "passes.h"
#include "symbol.h"
#include "expr.h"
#include "stmt.h"
#include "docs.h"
#include "mysystem.h"

int NUMTABS = 0;

static std::map<std::string, std::string> outputMap;
// If we were using C++11 then this could be an unordered_map

static int compareNames(const void* v1, const void* v2) {
  Symbol* s1 = *(Symbol* const *)v1;
  Symbol* s2 = *(Symbol* const *)v2;
  return strcmp(s1->name, s2->name);
}

static int compareClasses(const void *v1, const void* v2) {
  Type *t1 = *(Type* const *)v1;
  Type *t2 = *(Type* const *)v2;
  return strcmp(t1->symbol->name, t2->symbol->name);
}

void docs(void) {

  if (fDocs) {
    // Create a map of structural names to their expected chpldoc output
    if (fDocsTextOnly) {
      outputMap["class"] = "Class: ";
      outputMap["record"] = "Record: ";
      outputMap["config"] = "config ";
    } else {
      outputMap["class"] = ".. class:: ";
      outputMap["record"] = ".. record:: ";
      outputMap["module"] = ".. module:: ";
      outputMap["module comment prefix"] = ":synopsis: ";
      outputMap["iter func"] = ".. iterfunction:: ";
      outputMap["iter method"] = ".. itermethod:: ";
      outputMap["func"] = ".. function:: ";
      outputMap["method"] = ".. method:: ";
      outputMap["config"] = ".. data:: ";
      outputMap["field"] = ".. attribute:: ";
    }

    // Open the directory to store the docs
    std::string folderName = (strlen(fDocsFolder) != 0) ? fDocsFolder : "docs";


    mkdir(folderName.c_str(), S_IWUSR|S_IRUSR|S_IXUSR);
    
    forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
      if ((!devOnlyModule(mod) || developer) && !mod->hasFlag(FLAG_NO_DOC)) {
        std::string filename = mod->filename;

        if (mod->modTag == MOD_INTERNAL) {
          filename = "internal-modules/";
        } else if (mod ->modTag == MOD_STANDARD) {
          filename = "standard-modules/";
        } else {
          size_t location = filename.rfind("/");
          if (location != std::string::npos) {
            filename = filename.substr(0, location + 1);
          } else {
            filename = "";
          }
        }
        filename = folderName + "/" + filename;
        createDocsFileFolders(filename);
        
        if (isNotSubmodule(mod)) {
          // Creates files for each top level module
          if (!fDocsTextOnly)
            filename = filename + mod->name + ".rst";
          else
            filename = filename + mod->name + ".txt";
          std::ofstream file(filename.c_str(), std::ios::out);
          if (!fDocsTextOnly) {
            file << ".. default-domain:: chpl" << std::endl << std::endl;
          }
          printModule(&file, mod, mod->name);
          file.close();
        }
      }
    }
  }
}

bool isNotSubmodule(ModuleSymbol *mod) {
  return (mod->defPoint == NULL || 
          mod->defPoint->parentSymbol == NULL ||
          mod->defPoint->parentSymbol->name == NULL || 
          strcmp("chpl__Program", mod->defPoint->parentSymbol->name) == 0 ||
          strcmp("_root", mod->defPoint->parentSymbol->name) == 0);
}

void printIntent(std::ofstream *file, IntentTag intent) {
  switch(intent) {
  case INTENT_IN:
    *file << "in "; break;
  case INTENT_INOUT:
    *file << "inout "; break;
  case INTENT_OUT:
    *file << "out "; break;
  case INTENT_CONST:
    *file << "const "; break;
  case INTENT_CONST_IN:
    *file << "const in "; break;
  case INTENT_CONST_REF:
    *file << "const ref "; break;
  case INTENT_REF:
    *file << "ref "; break;
  case INTENT_PARAM:
    *file << "param "; break;
  default:
    break;
  }
}

void printArg(std::ofstream *file, ArgSymbol *arg) {
  printIntent(file, arg->intent);
  if (arg->hasFlag(FLAG_TYPE_VARIABLE)) {
    *file << "type ";  
    // Because type intents are handled differently during parsing
  } 
    
  *file << arg->name;
  if (arg->typeExpr != NULL) {
    *file << ": ";
    arg->typeExpr->body.tail->prettyPrint(file);
  } else if (arg->type != NULL && arg->type != dtAny) {
    *file << ": " << arg->type->symbol->name;
  }
}

void printFields(std::ofstream *file, AggregateType *cl) {
  for (int i = 1; i <= cl->fields.length; i++) {
    if (VarSymbol *var = toVarSymbol(((DefExpr *)cl->fields.get(i))->sym)) {
      if (!var->hasFlag(FLAG_SUPER_CLASS) &&
          !var->hasFlag(FLAG_NO_DOC)) {
        // Don't document the super class field, we don't want to know about it
        // Also, don't document this field if it has a "no doc" pragma attached
        // to it
        printTabs(file);
        *file << outputMap["field"];
        // For rst, this will insert '.. attribute:: ' here
        // For plain text, nothing will be inserted
        printVarStart(file, var);
        Expr *expr;
        if (cl->isClass()) {
          expr = cl->defaultTypeConstructor->body->body.get(i);
        } else {
          expr = cl->defaultTypeConstructor->body->body.get(i+1);
        }
        if (CallExpr *list = toCallExpr(expr)) {
          if (CallExpr *end = toCallExpr(list->argList.tail)) {
            if (end->primitive != NULL) {
              *file << ": ";
              end->prettyPrint(file);
              // TODO: prettify type output
            } else if (SymExpr* sym = toSymExpr(end->argList.tail)) {
              *file << " = ";
              sym->prettyPrint(file);
            }
          }
        } 
  
        *file << std::endl;
        printVarDocs(file, var);
      }
    }
  }
}

void inheritance(Vec<AggregateType*> *list, AggregateType *cl) {
  forv_Vec(Type, t, cl->dispatchParents) {
    if (AggregateType* c = toAggregateType(t)) {
      list->add_exclusive(c);
      inheritance(list, c);
    }
  }
}

void printClass(std::ofstream *file, AggregateType *cl) {
  if (! cl->isUnion() && !cl->symbol->hasFlag(FLAG_NO_DOC)) {
    printTabs(file);

    // TODO: for rst, change convert to '.. class:: ' and '.. record:: '
    if (cl->isClass()) {
      *file << outputMap["class"];
    } else if (cl->isRecord()) {
      *file << outputMap["record"];
    }
  
    NUMTABS++;
    *file << cl->symbol->name << std::endl;
    if (cl->doc != NULL) {
      printTabs(file);
      *file << cl->doc << std::endl;
    }
    printFields(file, cl);
    // If alphabetical option passed, alphabetizes the output
    if (fDocsAlphabetize) 
      qsort(cl->methods.v, cl->methods.n, sizeof(cl->methods.v[0]), 
        compareNames);
    
    forv_Vec(FnSymbol, fn, cl->methods){
      printFunction(file, fn, true);
    }
    
    Vec<AggregateType*> list;
    inheritance(&list, cl);

    if (fDocsAlphabetize)
      qsort(list.v, list.n, sizeof(list.v[0]), compareClasses);
    
    forv_Vec(AggregateType, c, list) {
      printTabs(file);
      *file << "inherited from " << c->symbol->name;
      *file << std::endl;
      NUMTABS++;
      printFields(file, c);
    
      forv_Vec(FnSymbol, fn, c->methods) {
        printFunction(file, fn, true);
       
      }
      NUMTABS--;
      *file << std::endl;
    }
    NUMTABS--;
  }
}

void printVarStart(std::ofstream *file, VarSymbol *var) {
  // TODO: I need to get this to print type fields as type fields instead of
  // vars.
  if (var->isConstant())
    *file << "const ";
  else if (var->isParameter())
    *file << "param ";
  else 
    *file << "var ";
  
  *file << var->name;
}

void printVarType(std::ofstream *file, VarSymbol *var) {  
  if (var->defPoint->exprType != NULL) {
    *file << ": ";
    var->defPoint->exprType->prettyPrint(file);
    // TODO: Make type output prettier
  }
  *file << std::endl;
}

void printVarDocs(std::ofstream *file, VarSymbol *var) {
  // TODO: Do we want to parse the output here to make it indent nicely?
  NUMTABS++;
  if (var->doc != NULL) {
    printTabs(file);
    *file << var->doc;
    *file << std::endl;
  }
  NUMTABS--;
}

void printTabs(std::ofstream *file) {
  for (int i = 1; i <= NUMTABS; i++) {
    *file << "   ";
  }
}

// Returns true if the provided fn is a module initializer, class constructor,
// type constructor, or module copy of a class method.  These functions are
// only printed in developer mode.  Is not applicable to printing class
// functions.
bool devOnlyFunction(FnSymbol *fn) {
  return (fn->hasFlag(FLAG_MODULE_INIT) || fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) 
          || fn->hasFlag(FLAG_CONSTRUCTOR) || fn->hasFlag(FLAG_METHOD));
}

// Returns true if the provide module is one of the internal or standard 
// modules. It is our opinion that these should only automatically be printed
// out if the user is in developer mode.
bool devOnlyModule(ModuleSymbol *mod) {
  return mod->modTag == MOD_INTERNAL || mod->modTag == MOD_STANDARD;
}

void printModule(std::ofstream *file, ModuleSymbol *mod, std::string name) {
  if (!mod->hasFlag(FLAG_NO_DOC)) {
    printTabs(file);
    *file << "Module: " << name << std::endl;
    if (!fDocsTextOnly) {
      int length = strlen("Module: ") + strlen(name.c_str());
      for (int i = 0; i < length; i++) {
        *file << "=";
      }
      *file << std::endl;
      // Make the length of this equal to "Module: " + name.length
    }
    NUMTABS++;
    if (mod->doc != NULL) {
      printTabs(file);
      *file << mod->doc << std::endl;
    }
    if(!fDocsTextOnly) {
      NUMTABS--;
      *file << outputMap["module"] << name << std::endl;
      if (mod->doc != NULL) {
        NUMTABS++;
        printTabs(file);
        *file << outputMap["module comment prefix"];
        *file << mod->doc << std::endl;
        *file << std::endl;
        NUMTABS--;
      }
    }

    Vec<VarSymbol*> configs = mod->getTopLevelConfigVars();
    if (fDocsAlphabetize)
      qsort(configs.v, configs.n, sizeof(configs.v[0]), compareNames);
    forv_Vec(VarSymbol, var, configs) {
      if (!var->hasFlag(FLAG_NO_DOC)) {
        printTabs(file);
        *file << outputMap["config"];
        printVarStart(file, var);
        printVarType(file, var);
        printVarDocs(file, var);
      }
    }

    Vec<FnSymbol*> fns = mod->getTopLevelFunctions(true);
    // If alphabetical option passed, fDocsAlphabetizes the output
    if (fDocsAlphabetize)
      qsort(fns.v, fns.n, sizeof(fns.v[0]), compareNames);
  
    forv_Vec(FnSymbol, fn, fns) {
      if (!devOnlyFunction(fn) || developer) {
        printFunction(file, fn, false);
      }
    }

    Vec<AggregateType*> classes = mod->getTopLevelClasses();
    if (fDocsAlphabetize)
      qsort(classes.v, classes.n, sizeof(classes.v[0]), compareClasses);

    forv_Vec(AggregateType, cl, classes) {
      printClass(file, cl);
    }

    Vec<ModuleSymbol*> mods = mod->getTopLevelModules();
    if (fDocsAlphabetize)
      qsort(mods.v, mods.n, sizeof(mods.v[0]), compareNames);
  
    forv_Vec(ModuleSymbol, md, mods) {
      if (!devOnlyModule(md) || developer)
        printModule(file, md, name + "." +  md->name);
    }
    if (fDocsTextOnly)
      NUMTABS--;
  }
}

void printFunction(std::ofstream *file, FnSymbol *fn, bool method) {
  if (!fn->hasFlag(FLAG_NO_DOC)) {
    printTabs(file);
    NUMTABS++;
    bool iterator = fn->hasFlag(FLAG_ITERATOR_FN);
    if (method) {
      if (iterator) {
        *file << outputMap["iter method"];
      } else {
        *file << outputMap["method"];
      }
    } else {
      if (iterator) {
        *file << outputMap["iter func"];
      } else {
        *file << outputMap["func"];
      }
    }
    if (fn->hasFlag(FLAG_INLINE)) {
      *file << "inline ";
    } else if (fn->hasFlag(FLAG_EXPORT)) {
      *file << "export ";
    } else if (fn->hasFlag(FLAG_EXTERN)) {
      *file << "extern ";
    }

    if (iterator) {
      *file << "iter ";
    } else {
      *file << "proc ";
    }

    *file << fn->name << "(";
    if (fn->numFormals() > 0) {
      if (!developer && strcmp(fn->getFormal(1)->name, "_mt") == 0) {
        for (int i = 3; i < fn->numFormals(); i++) {
          ArgSymbol *cur = fn->getFormal(i);
          printArg(file, cur);
          *file << ", ";
        }
        if (fn->numFormals() != 2) {
          ArgSymbol *cur = fn->getFormal(fn->numFormals());
          printArg(file, cur);
        }
      } else {
        for (int i = 1; i < fn->numFormals(); i++) {
          ArgSymbol *cur = fn->getFormal(i);
          printArg(file, cur);
          *file << ", ";
        }
        ArgSymbol *cur = fn->getFormal(fn->numFormals());
        printArg(file, cur);
      }
    }
    *file << ")";
    switch (fn->retTag) {
    case RET_REF:
      *file << " ref"; break;
    case RET_PARAM:
      *file << " param"; break;
    case RET_TYPE:
      *file << " type"; break;
    default: break;
    }
    if (fn->retExprType != NULL) {
      *file << ": ";
      fn->retExprType->body.tail->prettyPrint(file);
      // TODO: better type output
    }
    *file << std::endl;

    if (fn->doc != NULL) {
      printTabs(file);
      *file << fn->doc << std::endl;
    }
    *file << std::endl;
    NUMTABS--;
  }
}

void createDocsFileFolders(std::string filename) {
  size_t dirCutoff = filename.find("/");
  size_t total = 0;
  while (dirCutoff != std::string::npos) {
    // Creates each subdirectory within the new documentation directory
    dirCutoff += total;
    std::string shorter = filename.substr(dirCutoff+1);
    std::string otherHalf = filename.substr(0, dirCutoff);
    mkdir(otherHalf.c_str(), S_IWUSR|S_IRUSR|S_IXUSR);
    total = dirCutoff + 1;
    dirCutoff = shorter.find("/");
  }
}
