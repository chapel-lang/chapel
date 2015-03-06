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

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

#include "AstPrintDocs.h"
#include "AstToText.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "mysystem.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "stringutil.h"

#include "docs.h"

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

    // The location of intermediate rst files.
    std::string docsRstDir;
    if (fDocsTextOnly) {
      // For text-only mode, the output and working location is the same.
      docsRstDir = docsOutputDir;
    } else {
      // For rst mode, the working location is somewhere inside the temp dir.
      docsRstDir = generateSphinxProject(docsSphinxDir);
    }

    // TODO: Check for errors here... (thomasvandoren, 2015-02-25)
    mkdir(docsRstDir.c_str(), S_IWUSR|S_IRUSR|S_IXUSR);
    mkdir(docsOutputDir.c_str(), S_IWUSR|S_IRUSR|S_IXUSR);


    forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
      // TODO: Add flag to compiler to turn on doc dev only output
      if (!mod->hasFlag(FLAG_NO_DOC) && !devOnlyModule(mod)) {
        if (isNotSubmodule(mod)) {
          std::ofstream *file = openFileFromMod(mod, docsRstDir);

          AstPrintDocs *docsVisitor = new AstPrintDocs(file);
          mod->accept(docsVisitor);
          delete docsVisitor;

          // Comment the above three lines and uncomment the following line to
          // get the old category based output (or alphabetical). Note that
          // this will be restored (hopefully soon)... (thomasvandoren, 2015-02-22)
          //
          // printModule(file, mod, 0);

          file->close();
        }
      }
    }

    if (!fDocsTextOnly) {
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
  if (!cl->symbol->hasFlag(FLAG_NO_DOC) && ! cl->isUnion()) {
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

void printModule(std::ofstream *file, ModuleSymbol *mod, unsigned int tabs) {
  if (!mod->hasFlag(FLAG_NO_DOC)) {
    mod->printDocs(file, tabs);

    Vec<VarSymbol*> configs = mod->getTopLevelConfigVars();
    if (fDocsAlphabetize)
      qsort(configs.v, configs.n, sizeof(configs.v[0]), compareNames);
    forv_Vec(VarSymbol, var, configs) {
      var->printDocs(file, tabs + 1);
    }

    Vec<VarSymbol*> variables = mod->getTopLevelVariables();
    if (fDocsAlphabetize)
      qsort(variables.v, variables.n, sizeof(variables.v[0]), compareNames);
    forv_Vec(VarSymbol, var, variables) {
      var->printDocs(file, tabs + 1);
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
        fn->printDocs(file, tabs + 1);
      }
    }

    Vec<AggregateType*> classes = mod->getTopLevelClasses();
    if (fDocsAlphabetize)
      qsort(classes.v, classes.n, sizeof(classes.v[0]), compareClasses);

    forv_Vec(AggregateType, cl, classes) {
      printClass(file, cl, tabs + 1);
    }

    Vec<ModuleSymbol*> mods = mod->getTopLevelModules();
    if (fDocsAlphabetize)
      qsort(mods.v, mods.n, sizeof(mods.v[0]), compareNames);
  
    forv_Vec(ModuleSymbol, subMod, mods) {
      // TODO: Add flag to compiler to turn on doc dev only output
      if (!devOnlyModule(subMod)) {
        subMod->addPrefixToName(mod->docsName() + ".");
        printModule(file, subMod, tabs + 1);
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
  // Create the output dir under the docs output dir.
  const char * sphinxDir = dirpath.c_str();

  // Ensure output directory exists.
  const char * mkdirCmd = astr("mkdir -p ", sphinxDir);
  mysystem(mkdirCmd, "creating docs output dir");

  // Copy the sphinx template into the output dir.
  const char * sphinxTemplate = astr(CHPL_HOME, "/third-party/chpldoc-venv/chpldoc-sphinx-project/*");
  const char * cmd = astr("cp -r ", sphinxTemplate, " ", sphinxDir, "/");
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
  // based on the install path in the third-party/chpldoc-venv/ dir.
  const char * venvDir = astr(
    CHPL_HOME, "/third-party/chpldoc-venv/install/",
    CHPL_TARGET_PLATFORM, "/chpldoc-virtualenv");
  const char * venvBinDir = astr(venvDir, "/bin");
  const char * sphinxBuild = astr(venvBinDir, "/sphinx-build");
  const char * envVars = astr("export PATH=", venvBinDir, ":$PATH && ",
                              "export VIRTUAL_ENV=", venvDir);

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

  // Creates files for each top level module.
  if (fDocsTextOnly) {
    filename = filename + mod->name + ".txt";
  } else {
    filename = filename + mod->name + ".rst";
  }

  return filename;
}


std::ofstream* openFileFromMod(ModuleSymbol *mod, std::string docsWorkDir) {
  std::string filename = filenameFromMod(mod, docsWorkDir);
  return new std::ofstream(filename.c_str(), std::ios::out);
}
