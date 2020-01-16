/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "docs.h"

#include <algorithm>
#include <cerrno>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

#include "AstPrintDocs.h"
#include "AstToText.h"
#include "docsDriver.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "mysystem.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "stringutil.h"

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
    // Open the directory to store the docs

    // This is the final location for the output format (e.g. the html files.).
    std::string docsOutputDir;
    if (strlen(fDocsFolder) > 0) {
      docsOutputDir = fDocsFolder;
    } else {
      docsOutputDir = astr(getCwd(), "/docs");
    }

    // Root of the sphinx project and generated rst files. If
    // --docs-save-sphinx is not specified, it will be a temp dir.
    std::string docsTempDir = "";
    std::string docsSphinxDir;
    if (strlen(fDocsSphinxDir) > 0) {
      docsSphinxDir = fDocsSphinxDir;
    } else {
      docsTempDir = makeTempDir("chpldoc-");
      docsSphinxDir = docsTempDir;
    }

    // Make the intermediate dir and output dir.
    makeDir(docsSphinxDir.c_str());
    makeDir(docsOutputDir.c_str());

    // The location of intermediate rst files.
    std::string docsRstDir;
    if (fDocsTextOnly) {
      // For text-only mode, the output and working location is the same.
      docsRstDir = docsOutputDir;
    } else {
      // For rst mode, the working location is somewhere inside the temp dir.
      docsRstDir = generateSphinxProject(docsSphinxDir);
    }

    forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
      // TODO: Add flag to compiler to turn on doc dev only output
      if (!mod->noDocGen() && !devOnlyModule(mod)) {
        if (isNotSubmodule(mod)) {
          std::string filename = filenameFromMod(mod, docsRstDir);

          AstPrintDocs *docsVisitor = new AstPrintDocs(mod->name, filename, "");
          mod->accept(docsVisitor);
          delete docsVisitor;

          // Comment the above three lines and uncomment the following line to
          // get the old category based output (or alphabetical). Note that
          // this will be restored (hopefully soon)... (thomasvandoren, 2015-02-22)
          //
          // printModule(file, mod, 0, "");
        }
      }
    }

    if (!fDocsTextOnly && fDocsHTML) {
      generateSphinxOutput(docsSphinxDir, docsOutputDir);
    }

    if (docsTempDir.length() > 0) {
      deleteDir(docsTempDir.c_str());
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

void printFields(std::ofstream *file, AggregateType *cl, unsigned int tabs) {
  for (int i = 1; i <= cl->fields.length; i++) {
    if (VarSymbol *var = toVarSymbol(((DefExpr *)cl->fields.get(i))->sym)) {
      var->printDocs(file, tabs);
    }
  }
}

void printClass(std::ofstream *file, AggregateType *cl, unsigned int tabs) {
  if (!cl->symbol->noDocGen() && !cl->isUnion()) {
    cl->printDocs(file, tabs);

    printFields(file, cl, tabs + 1);

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
        fn->printDocs(file, tabs + 1);
    }
  }
}

// Returns true if the provided fn is a module initializer, type constructor,
// or module copy of a class method.  These functions are only printed in
// developer mode.  Is not applicable to printing class functions.
bool devOnlyFunction(FnSymbol *fn) {
  return (fn->hasFlag(FLAG_MODULE_INIT) || fn->isPrimaryMethod());
}

// Returns true if the provide module is one of the internal or standard 
// modules. It is our opinion that these should only automatically be printed
// out if the user is in developer mode.
bool devOnlyModule(ModuleSymbol *mod) {
  return mod->modTag == MOD_INTERNAL || mod->modTag == MOD_STANDARD;
}

void printModule(std::ofstream *file, ModuleSymbol *mod, unsigned int tabs, std::string parentName) {
  if (!mod->noDocGen()) {
    mod->printDocs(file, tabs, parentName);

    std::vector<VarSymbol*> configs = mod->getTopLevelConfigVars();
    if (fDocsAlphabetize)
      qsort(&configs[0], configs.size(), sizeof(VarSymbol*), compareNames);
    for_vector(VarSymbol, var, configs) {
      var->printDocs(file, tabs + 1);
    }

    std::vector<VarSymbol*> variables = mod->getTopLevelVariables();
    if (fDocsAlphabetize)
      qsort(&variables[0], variables.size(), sizeof(VarSymbol*), compareNames);
    for_vector(VarSymbol, var, variables) {
      var->printDocs(file, tabs + 1);
    }
    std::vector<FnSymbol*> fns = mod->getTopLevelFunctions(fDocsIncludeExterns);
    // If alphabetical option passed, fDocsAlphabetizes the output
    if (fDocsAlphabetize)
      qsort(&fns[0], fns.size(), sizeof(FnSymbol*), compareNames);
  
    for_vector(FnSymbol, fn, fns) {
      // TODO: Add flag to compiler to turn on doc dev only output

      // We want methods on classes that are defined at the module level to be
      // printed at the module level
      if (!devOnlyFunction(fn) || fn->isSecondaryMethod()) {
        fn->printDocs(file, tabs + 1);
      }
    }

    std::vector<AggregateType*> classes = mod->getTopLevelClasses();
    if (fDocsAlphabetize)
      qsort(&classes[0], classes.size(), sizeof(AggregateType*), compareClasses);

    for_vector(AggregateType, cl, classes) {
      printClass(file, cl, tabs + 1);
    }

    std::vector<ModuleSymbol*> mods = mod->getTopLevelModules();
    if (fDocsAlphabetize)
      qsort(&mods[0], mods.size(), sizeof(ModuleSymbol*), compareNames);
  
    for_vector(ModuleSymbol, subMod, mods) {
      // TODO: Add flag to compiler to turn on doc dev only output
      if (!devOnlyModule(subMod)) {
        std::string parent = "";
        if (parentName != "") {
          parent = parentName + ".";
        }
        parent = parent + mod->name;
        printModule(file, subMod, tabs + 1, parent);
      }
    }
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

    // Create `otherHalf` iff it is non-empty and does not already exist.
    if (otherHalf.length() > 0 && !existsAndDir(otherHalf.c_str())) {
      makeDir(otherHalf.c_str());
    }

    total = dirCutoff + 1;
    dirCutoff = shorter.find("/");
  }
}


/* Create the directory (non-recursively). If an error occurs, exit and report
 * error.
 */
static void makeDir(const char* dirpath) {
  static const int dirPerms = S_IRWXU | S_IRWXG | S_IRWXO;
  int result = mkdir(dirpath, dirPerms);
  if (result != 0 && errno != 0 && errno != EEXIST) {
    USR_FATAL(astr("Failed to create directory: ", dirpath,
                   " due to: ", strerror(errno)));
  }
}


/* Returns true if dirpath exists on file system and is a directory. */
static bool existsAndDir(const char* dirpath) {
  struct stat sb;
  return stat(dirpath, &sb) == 0 &&
    S_ISDIR(sb.st_mode);
}


/* 
 * Create new sphinx project at given location and return path where .rst files
 * should be placed.
 */
std::string generateSphinxProject(std::string dirpath) {
  // Create the output dir under the docs output dir.
  const char * sphinxDir = dirpath.c_str();

  // Copy the sphinx template into the output dir.
  const char * sphinxTemplate = astr(CHPL_HOME, "/third-party/chpl-venv/chpldoc-sphinx-project/*");
  const char * cmd = astr("cp -r ", sphinxTemplate, " ", sphinxDir, "/");
  if( printSystemCommands ) {
    printf("%s\n", cmd);
  }
  mysystem(cmd, "copying chpldoc sphinx template");

  const char * moddir = astr(sphinxDir, "/source/modules");
  return std::string(moddir);
}


/*
 * Invoke sphinx-build using sphinxDir to find conf.py and rst sources, and
 * outputDir for generated html files.
 */
void generateSphinxOutput(std::string sphinxDir, std::string outputDir) {
  // Set the PATH and VIRTUAL_ENV variables in the environment. The values are
  // based on the install path in the third-party/chpl-venv/ dir.

  const char * venvDir = astr(getVenvDir().c_str());
  const char * venvBinDir = astr(venvDir, "/bin");
  const char * sphinxBuild = astr("sphinx-build");

  const char * envVars = astr("export PATH=\"", venvBinDir, ":$PATH\" && "
                              "export VIRTUAL_ENV=", venvDir, " && "
                              "export CHPLDOC_AUTHOR='", fDocsAuthor, "'");

  // Run:
  //   $envVars &&
  //     sphinx-build -b html
  //     -d $sphinxDir/build/doctrees -W
  //     $sphinxDir/source $outputDir
  const char * cmdPrefix = astr(envVars, " && ");
  const char * cmd = astr(
    cmdPrefix,
    sphinxBuild, " -b html -d ",
    sphinxDir.c_str(), "/build/doctrees -W ",
    sphinxDir.c_str(), "/source ", outputDir.c_str());
  if( printSystemCommands ) {
    printf("%s\n", cmd);
  }
  mysystem(cmd, "building html output from chpldoc sphinx project");
  printf("HTML files are at: %s\n", outputDir.c_str());
}


std::string filenameFromMod(ModuleSymbol *mod, std::string docsWorkDir) {
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
  filename = docsWorkDir + "/" + filename;
  createDocsFileFolders(filename);

  return filename;
}
