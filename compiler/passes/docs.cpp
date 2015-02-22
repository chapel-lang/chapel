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
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

#include "driver.h"
#include "passes.h"
#include "symbol.h"
#include "expr.h"
#include "stmt.h"
#include "docs.h"
#include "mysystem.h"
#include "stringutil.h"
#include "scopeResolve.h"
#include "AstToText.h"

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
    // To handle inheritance, we need to look up parent classes and records.
    // In order to be successful when looking up these parents, the import
    // expressions should be accurately accounted for.
    addToSymbolTable(gDefExprs);
    processImportExprs();

    // Create a map of structural names to their expected chpldoc output
    if (fDocsTextOnly) {
      outputMap["class"] = "Class: ";
      outputMap["record"] = "Record: ";
    } else {
      outputMap["class"] = ".. class:: ";
      outputMap["record"] = ".. record:: ";
      outputMap["module"] = ".. module:: ";
      outputMap["module comment prefix"] = ":synopsis: ";
    }

    // Open the directory to store the docs
    std::string docsDir = (strlen(fDocsFolder) != 0) ? fDocsFolder : "docs";
    std::string folderName = docsDir;

    if (!fDocsTextOnly) {
      folderName = generateSphinxProject(docsDir);
    }

    mkdir(folderName.c_str(), S_IWUSR|S_IRUSR|S_IXUSR);
    
    forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
      // TODO: Add flag to compiler to turn on doc dev only output
      if (!mod->hasFlag(FLAG_NO_DOC) && !devOnlyModule(mod)) {
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

          printModule(&file, mod, mod->name);
          file.close();
        }
      }
    }

    if (!fDocsTextOnly) {
      generateSphinxOutput(docsDir);
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

void printFields(std::ofstream *file, AggregateType *cl) {
  for (int i = 1; i <= cl->fields.length; i++) {
    if (VarSymbol *var = toVarSymbol(((DefExpr *)cl->fields.get(i))->sym)) {
      var->makeField();
      var->printDocs(file, NUMTABS);
    }
  }
}

void inheritance(Vec<AggregateType*> *list, AggregateType *cl) {
  for_alist(expr, cl->inherits) {
    AggregateType* pt = discoverParentAndCheck(expr, cl);

    list->add_exclusive(pt);
    inheritance(list, pt);
  }
}

void printClass(std::ofstream *file, AggregateType *cl) {
  if (!cl->symbol->hasFlag(FLAG_NO_DOC) && ! cl->isUnion()) {
    printTabs(file);

    if (cl->isClass()) {
      *file << outputMap["class"];
    } else if (cl->isRecord()) {
      *file << outputMap["record"];
    }
  
    NUMTABS++;
    *file << cl->symbol->name << std::endl;

    // In rst mode, ensure there is an empty line between the class/record
    // signature and its description or the next directive.
    if (!fDocsTextOnly) {
      *file << std::endl;
    }

    if (cl->doc != NULL) {
      ltrimAndPrintLines(cl->doc, file);
      *file << std::endl;

      // In rst mode, ensure there is an empty line between the class/record
      // description and the next directive.
      if (!fDocsTextOnly) {
        *file << std::endl;
      }
    }
    printFields(file, cl);

    // In rst mode, add an additional line break after the attributes and
    // before the next directive.
    if (!fDocsTextOnly) {
      *file << std::endl;
    }

    // If alphabetical option passed, alphabetizes the output
    if (fDocsAlphabetize) 
      qsort(cl->methods.v, cl->methods.n, sizeof(cl->methods.v[0]), 
        compareNames);
    
    forv_Vec(FnSymbol, fn, cl->methods){
      // We only want to print methods defined within the class under the
      // class header
      if (fn->isPrimaryMethod())
        fn->printDocs(file, NUMTABS);
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
        fn->printDocs(file, NUMTABS);
       
      }
      NUMTABS--;
      *file << std::endl;
    }
    NUMTABS--;
  }
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
          || fn->hasFlag(FLAG_CONSTRUCTOR) || fn->isPrimaryMethod());
}

// Returns true if the provide module is one of the internal or standard 
// modules. It is our opinion that these should only automatically be printed
// out if the user is in developer mode.
bool devOnlyModule(ModuleSymbol *mod) {
  return mod->modTag == MOD_INTERNAL || mod->modTag == MOD_STANDARD;
}

void printModule(std::ofstream *file, ModuleSymbol *mod, std::string name) {
  if (!mod->hasFlag(FLAG_NO_DOC)) {
    if (!fDocsTextOnly) {
      *file << ".. default-domain:: chpl" << std::endl << std::endl;
    }

    // Print the module directive first, for .rst mode. This will associate the
    // Module: <name> title with the module. If the .. module:: directive comes
    // after the title, sphinx will complain about a duplicate id error.
    if (!fDocsTextOnly) {
      *file << outputMap["module"] << name << std::endl;
      if (mod->doc != NULL) {
        NUMTABS++;
        printTabs(file);
        *file << outputMap["module comment prefix"];

        // Grab first line of comment for synopsis.
        std::string firstLine = firstNonEmptyLine(mod->doc);
        *file << firstLine << std::endl;
        NUMTABS--;
      }
      *file << std::endl;
    }

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
      // Only print tabs for text only mode. The .rst prefers not to have the
      // tabs for module level comments and leading whitespace removed.
      if (fDocsTextOnly) {
        ltrimAndPrintLines(mod->doc, file);
      } else {
        NUMTABS--;
        ltrimAndPrintLines(mod->doc, file);
        *file << std::endl;
        NUMTABS++;
      }
    }
    // For non-rst mode, revert to the original tab level.
    if(!fDocsTextOnly) {
      NUMTABS--;
    }

    Vec<VarSymbol*> configs = mod->getTopLevelConfigVars();
    if (fDocsAlphabetize)
      qsort(configs.v, configs.n, sizeof(configs.v[0]), compareNames);
    forv_Vec(VarSymbol, var, configs) {
      var->printDocs(file, NUMTABS);
    }

    Vec<VarSymbol*> variables = mod->getTopLevelVariables();
    if (fDocsAlphabetize)
      qsort(variables.v, variables.n, sizeof(variables.v[0]), compareNames);
    forv_Vec(VarSymbol, var, variables) {
      var->printDocs(file, NUMTABS);
    }
    Vec<FnSymbol*> fns = mod->getTopLevelFunctions(fDocsIncludeExterns);
    // If alphabetical option passed, fDocsAlphabetizes the output
    if (fDocsAlphabetize)
      qsort(fns.v, fns.n, sizeof(fns.v[0]), compareNames);
  
    forv_Vec(FnSymbol, fn, fns) {
      // TODO: Add flag to compiler to turn on doc dev only output

      // We want methods on classes that are defined at the module level to be
      // printed at the module level
      if (!devOnlyFunction(fn) || fn->isSecondaryMethod()) {
        fn->printDocs(file, NUMTABS);
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
      // TODO: Add flag to compiler to turn on doc dev only output
      if (!devOnlyModule(md))
        printModule(file, md, name + "." +  md->name);
    }
    if (fDocsTextOnly)
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

/* 
 * Create new sphinx project at given location and return path where .rst files
 * should be placed.
 */
std::string generateSphinxProject(std::string dirpath) {
  // FIXME: This ought to be done in a TMPDIR, unless --save-rst is
  //        provided... (thomasvandoren, 2015-01-29)

  // Create the output dir under the docs output dir.
  const char * htmldir = astr(dirpath.c_str(), "/html");

  // Ensure output directory exists.
  const char * mkdirCmd = astr("mkdir -p ", htmldir);
  mysystem(mkdirCmd, "creating docs output dir");

  // Copy the sphinx template into the output dir.
  const char * sphinxTemplate = astr(CHPL_HOME, "/third-party/chpldoc-venv/chpldoc-sphinx-project/*");
  const char * cmd = astr("cp -r ", sphinxTemplate, " ", htmldir, "/");
  mysystem(cmd, "copying chpldoc sphinx template");

  const char * moddir = astr(htmldir, "/source/modules");
  return std::string(moddir);
}

/* Call `make html` from inside sphinx project. */
void generateSphinxOutput(std::string dirpath) {
  const char * htmldir = astr(dirpath.c_str(), "/html");

  // The virtualenv activate script is at:
  //   $CHPL_HOME/third-party/chpldoc-venv/install/$CHPL_TARGET_PLATFORM/chpldoc-virtualenv/bin/activate
  const char * activate = astr(
    CHPL_HOME, "/third-party/chpldoc-venv/install/",
    CHPL_TARGET_PLATFORM, "/chpdoc-virtualenv/bin/activate");

  // Run: `. $activate && cd $htmldir && $CHPL_MAKE html`
  const char * cmd = astr(
    ". ", activate,
    " && cd ", htmldir, " && ",
    CHPL_MAKE, " html");
  mysystem(cmd, "building html output from chpldoc sphinx project");
}


void ltrimAndPrintLines(std::string s, std::ofstream *file) {
  std::string trimmedS = ltrimAllLines(s);
  std::stringstream sStream(trimmedS);
  std::string line;
  while (std::getline(sStream, line)) {
    printTabs(file);
    *file << line;
    *file << std::endl;
  }
}
