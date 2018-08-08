/*
 * Copyright 2004-2018 Cray Inc.
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

#include "library.h"

#include "FnSymbol.h"
#include "beautify.h"
#include "codegen.h"
#include "driver.h"
#include "expr.h"
#include "stlUtil.h"
#include "stringutil.h"

char libDir[FILENAME_MAX + 1]  = "";

// TypeSymbol -> (pxdName, pyxName)  Will be "" if the cname should be used
std::map<TypeSymbol*, std::pair<std::string, std::string> > pythonNames;

static bool isFunctionToSkip(FnSymbol* fn);

//
// Generates a .h file to complement the library file created using --library
// This .h file will contain necessary #includes, any explicitly exported
// functions, and the module initialization function declarations.
//
void codegen_library_header(std::vector<FnSymbol*> functions) {
  if (fLibraryCompile) {
    fileinfo libhdrfile = { NULL, NULL, NULL };

    // Name the generated header file after the executable (and assume any
    // modifications to it have already happened)
    openLibraryHelperFile(&libhdrfile, libmodeHeadername, "h");
    // SIMPLIFYING ASSUMPTION: not handling LLVM just yet.  If were to, would
    // probably put assignment to gChplCompilationConfig here

    // follow convention of just not writing to the file if we can't open it
    if (libhdrfile.fptr != NULL) {
      FILE* save_cfile = gGenInfo->cfile;

      gGenInfo->cfile = libhdrfile.fptr;

      //genComment("Generated header file for use with %s",
      //           executableFilename);

      fprintf(libhdrfile.fptr, "#include \"stdchpl.h\"\n");

      // Maybe need something here to support LLVM extern blocks?

      // Print out the module initialization function headers and the exported
      // functions
      for_vector(FnSymbol, fn, functions) {
        if (fn->hasFlag(FLAG_EXPORT) &&
            !isFunctionToSkip(fn)) {
          fn->codegenPrototype();
        }
      }

      gGenInfo->cfile = save_cfile;
    }
    closeLibraryHelperFile(&libhdrfile);
  }
}

// Helper function to avoid unnecessary repetition when getting information
// from compileline
static std::string getCompilelineOption(std::string option) {
  std::string fullCommand = "$CHPL_HOME/util/config/compileline --" + option;
  fullCommand += "> cmd.out.tmp";
  runCommand(fullCommand);

  std::string replace = "$CHPL_HOME/util/config/replace-paths.py ";

  replace += "--fixpath '$(CHPL_RUNTIME_LIB)' $CHPL_RUNTIME_LIB ";
  replace += "--fixpath '$(CHPL_RUNTIME_INCL)' $CHPL_RUNTIME_INCL ";
  replace += "--fixpath '$(CHPL_THIRD_PARTY)' $CHPL_THIRD_PARTY ";
  replace += "--fixpath '$(CHPL_HOME)' $CHPL_HOME < cmd.out.tmp";

  std::string res = runCommand(replace);
  std::string cleanup = "rm cmd.out.tmp";
  runCommand(cleanup);
  return res;
}

// Save the value of the environment variable "var" into the makefile, so it
// can be referenced in the other variables for legibility purposes.
static void setupMakeEnvVars(std::string var, fileinfo makefile) {
  std::string cmd = "echo $";
  cmd += var;
  std::string cmdRes = runCommand(cmd);
  fprintf(makefile.fptr, "%s = %s\n", var.c_str(), cmdRes.c_str());
}

void codegen_library_makefile() {
  std::string name = "";
  int libLength = strlen("lib");
  bool startsWithLib = strncmp(executableFilename, "lib", libLength) == 0;
  if (startsWithLib) {
    name += &executableFilename[libLength];
  } else {
    // libname = executableFilename when executableFilename does not start with
    // "lib"
    name = executableFilename;
  }
  fileinfo makefile;
  openLibraryHelperFile(&makefile, "Makefile", name.c_str());

  // Save the CHPL_HOME location so it can be used in the other makefile
  // variables instead of letting them be cluttered with its value
  setupMakeEnvVars("CHPL_RUNTIME_LIB", makefile);
  setupMakeEnvVars("CHPL_RUNTIME_INCL", makefile);
  setupMakeEnvVars("CHPL_THIRD_PARTY", makefile);
  setupMakeEnvVars("CHPL_HOME", makefile);

  // TODO: compileline --includes-and-defines adds -I. to the list
  // automatically.  If the library is in a different directory from the
  // Makefile that is using this one, that won't be sufficient to find the
  // header file.  I will handle this with the output directory change
  std::string cflags = getCompilelineOption("cflags");
  cflags.erase(cflags.length() - 1); // remove trailing newline
  std::string includes = getCompilelineOption("includes-and-defines");
  fprintf(makefile.fptr, "CHPL_CFLAGS = -I%s %s %s\n",
          libDir,
          cflags.c_str(),
          includes.c_str());

  std::string libraries = getCompilelineOption("libraries");
  std::string libname = "-l";
  if (startsWithLib) {
    // libname = "-l<name>" when executableFilename = "lib<name>"
    libname += name;
  } else {
    // libname = executableFilename plus the extension when executableFilename
    // does not start with "lib"
    libname = libDir;
    libname += "/";
    libname += name;
    libname += getLibraryExtension();
  }
  // TODO: adjust for different location for the library, see earlier TODO
  fprintf(makefile.fptr, "CHPL_LDFLAGS = -L%s %s %s\n",
          libDir,
          libname.c_str(),
          libraries.c_str());

  std::string compiler = getCompilelineOption("compiler");
  fprintf(makefile.fptr, "CHPL_COMPILER = %s\n", compiler.c_str());

  std::string linker = getCompilelineOption("linker");
  fprintf(makefile.fptr, "CHPL_LINKER = %s\n", linker.c_str());

  std::string linkerShared = getCompilelineOption("linkershared");
  fprintf(makefile.fptr, "CHPL_LINKERSHARED = %s", linkerShared.c_str());

  closeLibraryHelperFile(&makefile, false);
}

const char* getLibraryExtension() {
  if (fLibraryCompile) {
    if (fLinkStyle==LS_DYNAMIC) return ".so";
    else return ".a";
  }
  return "";
}

void ensureLibDirExists() {
  if (libDir[0] == '\0') {
    const char* dir = "lib";
    INT_ASSERT(strlen(dir) < sizeof(libDir));
    strcpy(libDir, dir);
  }
  ensureDirExists(libDir, "ensuring --library-dir directory exists");
}

void
openLibraryHelperFile(fileinfo* fi, const char* name, const char* ext) {
  if (ext)
    fi->filename = astr(name, ".", ext);
  else
    fi->filename = astr(name);

  ensureLibDirExists();
  fi->pathname = astr(libDir, "/", fi->filename);
  openfile(fi, "w");
}

void closeLibraryHelperFile(fileinfo* fi, bool beautifyIt) {
  closefile(fi->fptr);
  //
  // We should beautify if we were asked to
  //
  if (beautifyIt)
    beautify(fi);
}

static void pxdEnd() {
  FILE* pxd = gGenInfo->cfile;

  // Do not "clean up" this file, as it removes leading tabs and those are
  // necessary.
  fprintf(pxd, "\ncdef extern from \"chpltypes.h\":\n");
  fprintf(pxd, "\tctypedef void* c_fn_ptr\n\n");

  fprintf(pxd, "cdef extern from \"chpl-init.h\":\n");
  fprintf(pxd, "\tvoid chpl_library_init(int argc, char* argv[])\n");
  fprintf(pxd, "\tvoid chpl_library_finalize()\n");
}

// Populate the pythonNames map with the translation for bools, differently sized
// integers, etc.
static void setupPythonTypeMap() {
  pythonNames[dtInt[INT_SIZE_8]->symbol] = std::make_pair("", "numpy.int8");
  pythonNames[dtInt[INT_SIZE_16]->symbol] = std::make_pair("", "numpy.int16");
  pythonNames[dtInt[INT_SIZE_32]->symbol] = std::make_pair("", "numpy.int32");
  pythonNames[dtInt[INT_SIZE_64]->symbol] = std::make_pair("", "numpy.int64");
  pythonNames[dtUInt[INT_SIZE_8]->symbol] = std::make_pair("", "numpy.uint8");
  pythonNames[dtUInt[INT_SIZE_16]->symbol] = std::make_pair("", "numpy.uint16");
  pythonNames[dtUInt[INT_SIZE_32]->symbol] = std::make_pair("", "numpy.uint32");
  pythonNames[dtUInt[INT_SIZE_64]->symbol] = std::make_pair("", "numpy.uint64");
  pythonNames[dtReal[FLOAT_SIZE_32]->symbol] = std::make_pair("", "numpy.float32");
  pythonNames[dtReal[FLOAT_SIZE_64]->symbol] = std::make_pair("double", "float");
  pythonNames[dtBool->symbol] = std::make_pair("bint", "bint");
  pythonNames[dtStringC->symbol] = std::make_pair("char *", "bytes");
  pythonNames[dtComplex[COMPLEX_SIZE_64]->symbol] =
              std::make_pair("float complex", "numpy.complex64");
  pythonNames[dtComplex[COMPLEX_SIZE_128]->symbol] =
              std::make_pair("double complex", "numpy.complex128");

  // TODO: Handle bigint (which should naturally match to Python's int)

}

void codegen_library_python(std::vector<FnSymbol*> functions) {
  if (fLibraryCompile && fLibraryPython) {
    setupPythonTypeMap();

    fileinfo pxd = { NULL, NULL, NULL };

    openLibraryHelperFile(&pxd, libmodeHeadername, "pxd");

    if (pxd.fptr != NULL) {
      FILE* save_cfile = gGenInfo->cfile;

      gGenInfo->cfile = pxd.fptr;

      fprintf(pxd.fptr, "from libc.stdint cimport *\n\n");

      fprintf(pxd.fptr, "cdef extern from \"%s.h\":\n", libmodeHeadername);

      for_vector(FnSymbol, fn, functions) {
        if (!isFunctionToSkip(fn)) {
          fn->codegenPXD();
        }
      }

      pxdEnd();
      gGenInfo->cfile = save_cfile;
    }
    // Don't "beautify", it will remove the tabs
    closeLibraryHelperFile(&pxd, false);
  }
}


// Skip this function if it is defined in an internal module, or if it is
// the generated main function
static bool isFunctionToSkip(FnSymbol* fn) {
  return fn->getModule()->modTag == MOD_INTERNAL ||
    fn->hasFlag(FLAG_GEN_MAIN_FUNC);
}
