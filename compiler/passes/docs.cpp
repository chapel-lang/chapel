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

int NUMTABS = 0;
std::string FOLDERNAME = "docs";

static int compareNames(const void* v1, const void* v2) {
  Symbol* s1 = *(Symbol**)v1;
  Symbol* s2 = *(Symbol**)v2;
  return strcmp(s1->name, s2->name);
}

static int compareClasses(const void *v1, const void* v2) {
  Type *t1 = *(Type**)v1;
  Type *t2 = *(Type**)v2;
  return strcmp(t1->symbol->name, t2->symbol->name);
}

void docs(void) {
  if (fdocs) {
    std::map<std::string, std::ofstream *> files; 
    // Stores the list of open files that can be written to.
    mkdir(FOLDERNAME.c_str(), S_IWUSR|S_IRUSR|S_IXUSR);
    
    forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
      if (!devOnlyModule(mod) || developer) {
    std::string filename = mod->filename;
    if (filename.find(".chpl") != std::string::npos) {
      // removes .chpl from the end of the file name
      filename = filename.substr(0, filename.find(".chpl"));
    }
    filename = FOLDERNAME + "/" + filename;
    filename += ".txt";
    
    createDocsFileFolders(filename);

    // If the file we want to write to was not previously created,
    // create it here.  
    if (files[mod->filename] == NULL) {
      files[mod->filename] = 
        new std::ofstream(filename.c_str(), std::ios::out);
    }
    
    if (isNotSubmodule(mod)) {
      printModule(files[mod->filename], mod, mod->name);
    }
      }
    }
    // Goes through and closes all the open files
    std::map<std::string, std::ofstream *>::iterator iter = files.begin();
    for (size_t i = 0; iter != files.end(); i++) {   
      (*(iter->second)).close();
      advance(iter, 1);
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
  } else if (arg->type != NULL) {
    if (strcmp(arg->type->symbol->name, "_any") != 0)
      *file << ": " << arg->type->symbol->name;
  }
}

void printFields(std::ofstream *file, ClassType *cl) {
  for_fields(tmp, cl) {
    if (VarSymbol* field = toVarSymbol(tmp)) {
      if (strcmp(field->name, "super") != 0) {
    printTabs(file);
    *file << "field ";
    printVar(file, field);
      }
    }
  }
}

void inheritance(Vec<ClassType*> *list, ClassType *cl) {
  forv_Vec(Type, t, cl->dispatchParents) {
    if (ClassType* c = toClassType(t)) {
      list->add_exclusive(c);
      inheritance(list, c);
    }
  }
}

void printClass(std::ofstream *file, ClassType *cl) {
  if (cl->classTag != CLASS_UNION) {
    printTabs(file);
    if (cl->classTag == CLASS_CLASS) {
      *file << "Class: " ;
    } else if (cl->classTag == CLASS_RECORD) {
      *file << "Record: ";
    }
  
    NUMTABS++;
    *file << cl->symbol->name << std::endl;
    if (cl->doc != NULL) {
      printTabs(file);
      *file << cl->doc << std::endl;
    }
    printFields(file, cl);
        // If alphabetical option passed, alphabetizes the output 
    if (alphabetize) 
      qsort(cl->methods.v, cl->methods.n, sizeof(cl->methods.v[0]), 
        compareNames);
    
    forv_Vec(FnSymbol, fn, cl->methods){
      printFunction(file, fn);
    }
    
    Vec<ClassType*> list;
    inheritance(&list, cl);

    if (alphabetize)
      qsort(list.v, list.n, sizeof(list.v[0]), compareClasses);
    
    forv_Vec(ClassType, c, list) {
      printTabs(file);
      *file << "inherited from " << c->symbol->name << std::endl;
      NUMTABS++;
      printFields(file, c);
    
      forv_Vec(FnSymbol, fn, c->methods) {
    printFunction(file, fn);
       
      }
      NUMTABS--;
      *file << std::endl;
    }
    NUMTABS--;
  }
}

void printVar(std::ofstream *file, VarSymbol *var) {
  if (var->isConstant())
    *file << "const ";
  if (var->isParameter())
    *file << "param ";

  *file << var->name;
  // Change to output type correctly
  if (strcmp(var->type->symbol->name, "_any") != 0) 
    *file << ": " << var->type->symbol->name; 
  *file << std::endl;
  NUMTABS++;
  if (var->doc != NULL) {
    printTabs(file);
    *file << var->doc << std::endl;
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
  return fn->hasFlag(FLAG_MODULE_INIT) || fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) 
    || fn->hasFlag(FLAG_CONSTRUCTOR) || fn->hasFlag(FLAG_METHOD);
}

// Returns true if the provide module is one of the internal or standard 
// modules. It is our opinion that these should only automatically be printed
// out if the user is in developer mode.
bool devOnlyModule(ModuleSymbol *mod) {
  return mod->modTag == MOD_INTERNAL || mod->modTag == MOD_STANDARD;
}

void printModule(std::ofstream *file, ModuleSymbol *mod, std::string name) {
  printTabs(file);
  *file << "Module: " << name << std::endl;
  NUMTABS++;
  if (mod->doc != NULL) {
    printTabs(file);
    *file << mod->doc << std::endl;
  }
  Vec<VarSymbol*> configs = mod->getConfigVars();
  if (alphabetize)
    qsort(configs.v, configs.n, sizeof(configs.v[0]), compareNames);
  forv_Vec(VarSymbol, var, configs) {
    printTabs(file);
    *file << "config ";
    printVar(file, var);
  }

  Vec<FnSymbol*> fns = mod->getFunctions();
  // If alphabetical option passed, alphabetizes the output 
  if (alphabetize) 
    qsort(fns.v, fns.n, sizeof(fns.v[0]), compareNames);
  
  forv_Vec(FnSymbol, fn, fns) {
    if (!devOnlyFunction(fn) || developer) {
      printFunction(file, fn);
    }
  }

  Vec<ClassType*> classes = mod->getClasses();
  if (alphabetize)
    qsort(classes.v, classes.n, sizeof(classes.v[0]), compareClasses);

  forv_Vec(ClassType, cl, classes) {
    printClass(file, cl);
  }

  Vec<ModuleSymbol*> mods = mod->getModules();
  if (alphabetize)
    qsort(mods.v, mods.n, sizeof(mods.v[0]), compareNames);
  
  forv_Vec(ModuleSymbol, md, mods) {
    if (!devOnlyModule(md) || developer) 
      printModule(file, md, name + "." +  md->name);
  }
  NUMTABS--;
}

void printFunction(std::ofstream *file, FnSymbol *fn) {
  printTabs(file);
  NUMTABS++;
  if (fn->hasFlag(FLAG_INLINE)) {
    *file << "inline ";
  } else if (fn->hasFlag(FLAG_EXPORT)) {
    *file << "export ";
  } else if (fn->hasFlag(FLAG_EXTERN)) {
    *file << "extern ";
  }
  
  if (fn->hasFlag(FLAG_ITERATOR_FN)) {
    *file << "iter ";
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
  case RET_VAR:
    *file << " var"; break;
  case RET_PARAM:
    *file << " param"; break;
  case RET_TYPE:
    *file << " type"; break;
  default: break;
  }
  if (fn->retExprType != NULL) {
    *file << ": ";
    fn->retExprType->body.tail->prettyPrint(file);
  }
  *file << std::endl;

  if (fn->doc != NULL) {
    printTabs(file);
    *file << fn->doc << std::endl;
  }
  *file << std::endl;
  NUMTABS--;
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
