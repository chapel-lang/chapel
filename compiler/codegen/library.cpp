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

#include <cstring>

#include "library.h"

#include "FnSymbol.h"
#include "beautify.h"
#include "codegen.h"
#include "driver.h"
#include "expr.h"
#include "stlUtil.h"
#include "stringutil.h"

std::map<Symbol*, TypeSymbol*> exportedArrayElementType;

char libDir[FILENAME_MAX + 1]  = "";
std::string pxdName = "";

// TypeSymbol -> (pxdName, pyxName)  Will be "" if the cname should be used
std::map<TypeSymbol*, std::pair<std::string, std::string> > pythonNames;
std::map<TypeSymbol*, std::string> fortranKindNames;
std::map<TypeSymbol*, std::string> fortranTypeNames;

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

      int filenum = 0;
      while (const char* inputFilename = nthFilename(filenum++)) {
        if (isCHeader(inputFilename)) {
          fprintf(libhdrfile.fptr, "#include \"%s\"\n", inputFilename);
        }
      }

      if (fMultiLocaleInterop) {
        // If we've created a multilocale library, memory that is returned to
        // the client code wasn't created by chpl_mem_alloc and friends, so
        // shouldn't be freed using the normal strategies.  But, for
        // convenience, allow the user to still call `chpl_free`.
        fprintf(libhdrfile.fptr, "#define chpl_free(ptr) free(ptr)\n");
      }
      // Maybe need something here to support LLVM extern blocks?

      // Print out the module initialization function headers and the exported
      // functions
      for_vector(FnSymbol, fn, functions) {
        if (fn->hasFlag(FLAG_EXPORT) &&
            isUserRoutine(fn)) {
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
  std::string fullCommand = "";
  for (std::map<std::string, const char*>::iterator env=envMap.begin();
       env!=envMap.end(); ++env) {
    fullCommand += std::string(env->first) + "=\"" + std::string(env->second) +
      "\" ";
  }
  fullCommand += "$CHPL_HOME/util/config/compileline --" + option;
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
static void setupMakeEnvVars(std::string var, const char* value,
                             fileinfo makefile) {
  fprintf(makefile.fptr, "%s = %s\n\n", var.c_str(), value);
}

static void printMakefileIncludes(fileinfo makefile);
static void printMakefileLibraries(fileinfo makefile, std::string name);

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
  setupMakeEnvVars("CHPL_RUNTIME_LIB", CHPL_RUNTIME_LIB, makefile);
  setupMakeEnvVars("CHPL_RUNTIME_INCL", CHPL_RUNTIME_INCL, makefile);
  setupMakeEnvVars("CHPL_THIRD_PARTY", CHPL_THIRD_PARTY, makefile);
  setupMakeEnvVars("CHPL_HOME", CHPL_HOME, makefile);

  printMakefileIncludes(makefile);
  printMakefileLibraries(makefile, name);

  std::string compiler = getCompilelineOption("compiler");
  fprintf(makefile.fptr, "CHPL_COMPILER = %s\n", compiler.c_str());

  std::string linker = getCompilelineOption("linker");
  fprintf(makefile.fptr, "CHPL_LINKER = %s\n", linker.c_str());

  std::string linkerShared = getCompilelineOption("linkershared");
  fprintf(makefile.fptr, "CHPL_LINKERSHARED = %s", linkerShared.c_str());

  closeLibraryHelperFile(&makefile, false);
}

// Returns a string containing -I includes for every directory in the incDirs
// vector (which is populated by require statements)
static std::string getRequireIncludes() {
  std::string res = "";
  for_vector(const char, dirName, incDirs) {
    res += " -I";
    res += dirName;
  }
  return res;
}

// Returns a string containing -L includes for every directory containing
// library files specified in require statements, and -l references to those
// libraries
static std::string getRequireLibraries() {
  std::string res = "";
  // Adds the locations of the libraries specified using require statements
  for_vector(const char, dirName, libDirs) {
    res += " -L";
    res += dirName;
  }
  for_vector(const char, libName, libFiles) {
    res += " -l";
    res += libName;
  }

  return res;
}

// Helper to output the CHPL_CFLAGS variable into the generated makefile
static void printMakefileIncludes(fileinfo makefile) {
  std::string cflags = getCompilelineOption("cflags");
  cflags.erase(cflags.length() - 1); // remove trailing newline

  std::string requireIncludes = getRequireIncludes();

  std::string includes = getCompilelineOption("includes-and-defines");
  fprintf(makefile.fptr, "CHPL_CFLAGS = -I%s %s",
          libDir,
          cflags.c_str());

  if (requireIncludes != "") {
    fprintf(makefile.fptr, "%s", requireIncludes.c_str());
  }
  fprintf(makefile.fptr, " %s\n", includes.c_str());
}

// Helper to transform the provided name into library form for a compile line
// (-lname in the where the library starts with lib, loc/name.ext for when
// the library does not start with lib)
static std::string getLibname(std::string name) {
  std::string libname = "-l";
  libname += name;
  return libname;
}

// Helper to output the CHPL_LDFLAGS variable into the generated makefile
static void printMakefileLibraries(fileinfo makefile, std::string name) {
  std::string libraries = getCompilelineOption("libraries");
  std::string libname = getLibname(name);

  std::string requires = getRequireLibraries();

  fprintf(makefile.fptr, "CHPL_LDFLAGS = -L%s %s",
          libDir,
          libname.c_str());

  //
  // Multi-locale libraries require some extra libraries to be linked in order
  // to function correctly. For static libraries in particular, rather than
  // try to link these dependencies at compile time, we shunt responsibility
  // off to the user via use of `--library-makefile`.
  //
  if (fMultiLocaleInterop) {
    std::string deps = getCompilelineOption("multilocale-lib-deps");
    removeTrailingNewlines(deps);
    fprintf(makefile.fptr, " %s", deps.c_str());
  }

  if (requires != "") {
    fprintf(makefile.fptr, "%s", requires.c_str());
  }

  //
  // Append the Chapel library as the last linker argument. We do this as a
  // stopgap to make the GNU linker happy.
  //
  removeTrailingNewlines(libraries);
  fprintf(makefile.fptr, " %s %s\n\n", libraries.c_str(), libname.c_str());
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

    //
    // When compiling Python, the default name of the directory where
    // generated library files are stored is as same as the Python
    // module name.
    //
    const char* dir = fLibraryPython ? pythonModulename : "lib";
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
  pythonNames[dtStringC->symbol] = std::make_pair("const char *", "bytes");
  pythonNames[dtString->symbol] = std::make_pair("", "object");
  pythonNames[dtBytes->symbol] = std::make_pair("", "object");
  pythonNames[dtComplex[COMPLEX_SIZE_64]->symbol] =
              std::make_pair("float complex", "numpy.complex64");
  pythonNames[dtComplex[COMPLEX_SIZE_128]->symbol] =
              std::make_pair("double complex", "numpy.complex128");

  // TODO: Handle bigint (which should naturally match to Python's int)

}

// If there is a known .pxd file translation for this type, use that.
// Otherwise, use the normal cname
std::string getPythonTypeName(Type* type, PythonFileType pxd) {
  std::pair<std::string, std::string> tNames = pythonNames[type->symbol];
  if (pxd == C_PXD && tNames.first != "") {
    return tNames.first;
  } else if (pxd == PYTHON_PYX && tNames.second != "") {
    return tNames.second;
  } else if (pxd == C_PYX && (tNames.second != "" || tNames.first != "")) {
    std::string res = tNames.second;
    if (strncmp(res.c_str(), "numpy", strlen("numpy")) == 0) {
      res += "_t";
    } else if (strcmp(res.c_str(), "object") == 0) {
      // Types like byte and string map to Python objects that have no 1-to-1
      // representation in C.
      return res;
    } else {
      res = getPythonTypeName(type, C_PXD);
    }
    return res;
  } else {
    if (type->symbol->hasFlag(FLAG_REF)) {
      Type* referenced = type->getValType();
      std::string base = getPythonTypeName(referenced, pxd);
      if (pxd == C_PYX) {
        return "";
      } else {
        return base + " *";
      }
    } else if (type->symbol->hasFlag(FLAG_C_PTR_CLASS)) {
      Type* pointedTo = getDataClassType(type->symbol)->typeInfo();
      std::string base = getPythonTypeName(pointedTo, pxd);
      if (pxd == C_PYX) {
        return "";
      } else {
        return base + " *";
      }
    } else {
      return type->codegen().c;
    }
  }
}

static void setupFortranTypeMap() {
  fortranKindNames[dtInt[INT_SIZE_8]->symbol] = "c_int8_t";
  fortranKindNames[dtInt[INT_SIZE_16]->symbol] = "c_int16_t";
  fortranKindNames[dtInt[INT_SIZE_32]->symbol] = "c_int32_t";
  fortranKindNames[dtInt[INT_SIZE_64]->symbol] = "c_int64_t";

  // No unsigned integers in Fortran.  Fake it with signed integers.
  fortranKindNames[dtUInt[INT_SIZE_8]->symbol] = "c_int8_t";
  fortranKindNames[dtUInt[INT_SIZE_16]->symbol] = "c_int16_t";
  fortranKindNames[dtUInt[INT_SIZE_32]->symbol] = "c_int32_t";
  fortranKindNames[dtUInt[INT_SIZE_64]->symbol] = "c_int64_t";

  fortranKindNames[dtReal[FLOAT_SIZE_32]->symbol] = "c_float";
  fortranKindNames[dtReal[FLOAT_SIZE_64]->symbol] = "c_double";
  fortranKindNames[dtBool->symbol] = "c_bool";

  // Should any chapel type map to Fortran's `character(kind=c_char)`?
  //fortranKindNames[dtStringC->symbol] = "c_char";
  fortranKindNames[dtComplex[COMPLEX_SIZE_64]->symbol] = "c_float_complex";
  fortranKindNames[dtComplex[COMPLEX_SIZE_128]->symbol] = "c_double_complex";

  fortranTypeNames[dtInt[INT_SIZE_8]->symbol] = "integer";
  fortranTypeNames[dtInt[INT_SIZE_16]->symbol] = "integer";
  fortranTypeNames[dtInt[INT_SIZE_32]->symbol] = "integer";
  fortranTypeNames[dtInt[INT_SIZE_64]->symbol] = "integer";
  fortranTypeNames[dtUInt[INT_SIZE_8]->symbol] = "integer";
  fortranTypeNames[dtUInt[INT_SIZE_16]->symbol] = "integer";
  fortranTypeNames[dtUInt[INT_SIZE_32]->symbol] = "integer";
  fortranTypeNames[dtUInt[INT_SIZE_64]->symbol] = "integer";
  fortranTypeNames[dtReal[FLOAT_SIZE_32]->symbol] = "real";
  fortranTypeNames[dtReal[FLOAT_SIZE_64]->symbol] = "real";
  fortranTypeNames[dtBool->symbol] = "logical";
  // Should any chapel type map to Fortran's `character(kind=c_char)`?
  //fortranTypeNames[dtStringC->symbol] = "character";
  fortranTypeNames[dtComplex[COMPLEX_SIZE_64]->symbol] = "complex";
  fortranTypeNames[dtComplex[COMPLEX_SIZE_128]->symbol] = "complex";
}

static void makeFortranModule(std::vector<FnSymbol*> functions);
static void makePXDFile(std::vector<FnSymbol*> functions);
static void makePYXFile(std::vector<FnSymbol*> functions);
static void makePYFile();
static void makePYInitFile();

void codegen_library_python(std::vector<FnSymbol*> functions) {
  if (fLibraryCompile && fLibraryPython) {
    setupPythonTypeMap();

    pxdName = "chpl_";
    pxdName += libmodeHeadername;

    makePXDFile(functions);
    makePYXFile(functions);
    makePYFile();
    makePYInitFile();
  }
}

void codegen_library_fortran(std::vector<FnSymbol*> functions) {
  if (fLibraryCompile && fLibraryFortran) {
    setupFortranTypeMap();
    makeFortranModule(functions);
  }
}

void makeFortranModule(std::vector<FnSymbol*> functions) {
  const char* filename = fortranModulename[0] != '\0' ? fortranModulename
                                                      : libmodeHeadername;
  int indent = 0;
  fileinfo fort = { NULL, NULL, NULL };

  openLibraryHelperFile(&fort, filename, "f90");

  if (fort.fptr != NULL) {
    FILE* save_cfile = gGenInfo->cfile;
    gGenInfo->cfile = fort.fptr;
    fprintf(fort.fptr, "%*smodule %s\n", indent, "", filename);
    indent += 2;
    fprintf(fort.fptr, "%*suse ISO_C_BINDING\n", indent, "");
    fprintf(fort.fptr, "%*sinterface\n", indent, "");
    indent += 2;
    // generate chpl_library_init and chpl_library_finalize here?
    for_vector(FnSymbol, fn, functions) {
      if (isUserRoutine(fn)) {
        fn->codegenFortran(indent);
      }
    }

    indent -= 2;
    fprintf(fort.fptr, "%*send interface\n", indent, "");
    indent -= 2;
    fprintf(fort.fptr, "%*send module %s\n", indent, "", filename);
    gGenInfo->cfile = save_cfile;
  }

  closeLibraryHelperFile(&fort, false);
}

// Generate the .pxd file for the library.  This will be used when creating
// the Python module
static void makePXDFile(std::vector<FnSymbol*> functions) {
  fileinfo pxd = { NULL, NULL, NULL };

  openLibraryHelperFile(&pxd, pxdName.c_str(), "pxd");

  if (pxd.fptr != NULL) {
    FILE* save_cfile = gGenInfo->cfile;

    gGenInfo->cfile = pxd.fptr;

    fprintf(pxd.fptr, "from libc.stdint cimport *\n");
    // Get the permanent runtime definitions
    fprintf(pxd.fptr, "from chplrt cimport *\n\n");

    fprintf(pxd.fptr, "cdef extern from \"%s.h\":\n", libmodeHeadername);

    for_vector(FnSymbol, fn, functions) {
      if (isUserRoutine(fn)) {
        fn->codegenPython(C_PXD);
      }
    }

    gGenInfo->cfile = save_cfile;
  }
  // Don't "beautify", it will remove the tabs
  closeLibraryHelperFile(&pxd, false);
}

static void makePYXSetupFunctions(std::vector<FnSymbol*> moduleInits);
static void makeOpaqueArrayClass();

// Generate the .pyx file for the library.  This will also be used when
// creating the Python module.
static void makePYXFile(std::vector<FnSymbol*> functions) {
  fileinfo pyx = { NULL, NULL, NULL };

  openLibraryHelperFile(&pyx, pythonModulename, "pyx");

  if (pyx.fptr != NULL) {
    FILE* save_cfile = gGenInfo->cfile;

    gGenInfo->cfile = pyx.fptr;

    fprintf(pyx.fptr, "#!python\n");
    fprintf(pyx.fptr, "#cython: language_level=3\n");
    // Make import statement at top of .pyx file for chpl_library_init and
    // chpl_library_finalize
    fprintf(pyx.fptr, "from chplrt cimport chpl_library_init, ");
    fprintf(pyx.fptr, "chpl_library_finalize, chpl_external_array, ");
    fprintf(pyx.fptr, "chpl_make_external_array, chpl_make_external_array_ptr");
    fprintf(pyx.fptr, ", chpl_free_external_array, chpl_opaque_array, ");
    fprintf(pyx.fptr, "cleanupOpaqueArray, chpl_free, ");
    fprintf(pyx.fptr, "chpl_byte_buffer, chpl_byte_buffer_free, ");
    fprintf(pyx.fptr, "PyBytes_FromStringAndSize\n");

    std::vector<FnSymbol*> moduleInits;
    std::vector<FnSymbol*> exported;

    fprintf(pyx.fptr, "from %s cimport ", pxdName.c_str());
    bool first = true;
    // Make import statement at top of .pyx file for exported functions
    for_vector(FnSymbol, fn, functions) {
      if (isUserRoutine(fn)) {
        if (fn->hasFlag(FLAG_EXPORT)) {
          if (first) {
            first = false;
          } else {
            fprintf(pyx.fptr, ", ");
          }

          // Module initialization functions get handled together, other
          // exported functions will have their own definition.
          if (fn->hasFlag(FLAG_MODULE_INIT)) {
            // No need to rename the module init function, the user won't see it
            fprintf(pyx.fptr, "%s", fn->cname);
            moduleInits.push_back(fn);
          } else {
            // On import, rename the exported function so that we can use its
            // original name in the Python module to avoid confusion.
            fprintf(pyx.fptr, "%s as chpl_%s", fn->cname, fn->cname);
            exported.push_back(fn);
          }
        }
      }
    }
    fprintf(pyx.fptr, "\n\n");

    // Necessary for using numpy types
    fprintf(pyx.fptr, "import numpy\n");
    fprintf(pyx.fptr, "cimport numpy\n");
    // Necessary for supporting pointers
    fprintf(pyx.fptr, "import ctypes\n");
    fprintf(pyx.fptr, "from libc.stdint cimport intptr_t\n\n");

    makePYXSetupFunctions(moduleInits);
    makeOpaqueArrayClass();

    // Add Python wrapper for the exported functions, to translate the types
    // appropriately
    for_vector(FnSymbol, fn, exported) {
      fn->codegenPython(PYTHON_PYX);
    }

    gGenInfo->cfile = save_cfile;
  }
  // Don't "beautify", it will remove the tabs
  closeLibraryHelperFile(&pyx, false);
}

// Create definitions for chpl_setup and chpl_cleanup in the generated .pyx
// file
static void makePYXSetupFunctions(std::vector<FnSymbol*> moduleInits) {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;

  fprintf(outfile, "_chpl_cleanup_callback = None\n");
  fprintf(outfile, "\n");

  // Use to set hidden debug callback.
  fprintf(outfile, "def chpl_set_cleanup_callback(callback):\n");
  fprintf(outfile, "\tglobal _chpl_cleanup_callback\n");
  fprintf(outfile, "\t_chpl_cleanup_callback = callback\n");
  fprintf(outfile, "\n");

  // Initialize the runtime.  chpl_setup should get called prior to using
  // any of the exported functions
  if (fMultiLocaleInterop) {
    // Multilocale libraries need to take in the number of locales to use as
    // an argument

    // numLocales is a C default-sized int.
    std::string numLocalesType = getPythonTypeName(dtInt[INT_SIZE_32],
                                                   C_PYX);
    fprintf(outfile, "def chpl_setup(%s numLocales):\n",
            numLocalesType.c_str());
    fprintf(outfile,
            "\tcdef char** args = ['%s', '-nl', str(numLocales).encode()]\n",
            libmodeHeadername);
    // TODO: is there a way to get the number of indices from args?
    fprintf(outfile, "\tchpl_library_init(3, args)\n");

  } else {
    // Define `chpl_setup` for single locale Python modules.
    fprintf(outfile, "def chpl_setup():\n");
    fprintf(outfile, "\tcdef char** args = ['%s']\n", libmodeHeadername);
    fprintf(outfile, "\tchpl_library_init(1, args)\n");
  }

  // Initialize the included modules (continuation of chpl_setup definition)
  for_vector(FnSymbol, fn, moduleInits) {
    fprintf(outfile, "\t%s(1, 1)\n", fn->cname);
  }
  fprintf(outfile, "\n");

  // Shut down the runtime and libraries.  chpl_cleanup should get called when
  // the exported Chapel code is no longer needed
  fprintf(outfile, "def chpl_cleanup():\n");
  fprintf(outfile, "\tglobal _chpl_cleanup_callback\n");
  fprintf(outfile, "\tcallback = _chpl_cleanup_callback\n");
  fprintf(outfile, "\tif not callback is None:\n\t\tcallback()\n");
  fprintf(outfile, "\tchpl_library_finalize()\n\n");
}

static void makeOpaqueArrayClass() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;

  fprintf(outfile, "cdef class ChplOpaqueArray:\n");
  fprintf(outfile, "\tcdef chpl_opaque_array val\n\n");
  fprintf(outfile, "\tcdef inline setVal(self, chpl_opaque_array val):\n");
  fprintf(outfile, "\t\tself.val = val\n\n");

  fprintf(outfile, "\tdef cleanup(self):\n");
  fprintf(outfile, "\t\tcleanupOpaqueArray(&self.val);\n\n");

  // Allows the Python type to be created and cleaned up appropriately in a
  // Python "with" clause
  fprintf(outfile, "\tdef __enter__(self):\n");
  fprintf(outfile, "\t\treturn self\n\n");
  fprintf(outfile, "\tdef __exit__(self, exc_type, exc_value, traceback):\n");
  fprintf(outfile, "\t\tself.cleanup()\n\n");
}

// create the Python file which will be used to compile the .pyx, .pxd, library,
// and header files into a Python module.
static void makePYFile() {
  fileinfo py = { NULL, NULL, NULL };

  openLibraryHelperFile(&py, pythonModulename, "py");

  if (py.fptr != NULL) {
    FILE* save_cfile = gGenInfo->cfile;

    gGenInfo->cfile = py.fptr;

    std::string libname = "";
    int libLength = strlen("lib");
    bool startsWithLib = strncmp(executableFilename, "lib", libLength) == 0;
    if (startsWithLib) {
      libname += &executableFilename[libLength];
    } else {
      libname = executableFilename;
    }

    // Imports
    fprintf(py.fptr, "from distutils.core import setup\n");
    fprintf(py.fptr, "from distutils.core import Extension\n");
    fprintf(py.fptr, "from Cython.Build import cythonize\n");
    fprintf(py.fptr, "import numpy\n\n");

    // Get the static Chapel runtime and third-party libraries
    fprintf(py.fptr, "chpl_libraries=[");
    bool first = true;
    // Get the libraries listed in require statements
    for_vector(const char, libName, libFiles) {
      if (first) {
        first = false;
      } else {
        fprintf(py.fptr, ", ");
      }
      fprintf(py.fptr, "\"%s\"", libName);
    }
    std::string libraries = getCompilelineOption("libraries");

    // Erase trailing newline and append multilocale-only dependencies.
    if (fMultiLocaleInterop) {
      libraries.erase(libraries.length() - 1);
      libraries += " ";
      libraries += getCompilelineOption("multilocale-lib-deps");
    }

    char copyOfLib[libraries.length() + 1];
    libraries.copy(copyOfLib, libraries.length(), 0);
    copyOfLib[libraries.length()] = '\0';
    int prefixLen = strlen("-l");
    char* curSection = strtok(copyOfLib, " \n");
    // Get the libraries from compileline --libraries, taking the `name`
    // portion from all `-lname` parts of that command's output
    while (curSection != NULL) {
      if (strncmp(curSection, "-l", prefixLen) == 0) {
        if (first) {
          first = false;
        } else {
          fprintf(py.fptr, ", ");
        }
        fprintf(py.fptr, "\"%s\"", &curSection[prefixLen]);
      }
      curSection = strtok(NULL, " \n");
    }

    // Fetch addition
    fprintf(py.fptr, "]\n");

    // Cythonize me, Captain!
    fprintf(py.fptr, "setup(name = '%s library',\n", pythonModulename);
    fprintf(py.fptr, "\text_modules = cythonize(\n");
    fprintf(py.fptr, "\t\tExtension(\"%s\",\n", pythonModulename);
    fprintf(py.fptr, "\t\t\tinclude_dirs=[numpy.get_include()],\n");
    fprintf(py.fptr, "\t\t\tsources=[\"%s.pyx\"],\n", pythonModulename);
    fprintf(py.fptr, "\t\t\tlibraries=[\"%s\"] + chpl_libraries + "
                     "[\"%s\"])))\n",
                     libname.c_str(), libname.c_str());

    gGenInfo->cfile = save_cfile;
  }
  // Don't "beautify", it will remove the tabs
  closeLibraryHelperFile(&py, false);
}

static void makePYInitFile() {
  fileinfo py = { NULL, NULL, NULL };

  char* path = dirHasFile(libDir, "__init__.py");
  if (path != NULL) {
    free(path);
    USR_WARN("Cannot generate %s/__init__.py because it would overwrite "
             "existing file", libDir);
    return;
  }
 
  openLibraryHelperFile(&py, "__init__", "py");

  if (py.fptr != NULL) {
    FILE* save_cfile = gGenInfo->cfile;
    gGenInfo->cfile = py.fptr;

    //
    // Print the following form to the __init__.py file>
    //
    //    | """Generated by the Chapel compiler."""
    //    |
    //    | import atexit
    //    |
    //    | from <libraryDir>.<moduleName> import *
    //    |
    //    | # Register cleanup function to be called at program exit.
    //    | atexit.register(<moduleName>.chpl_cleanup)
    //    |
    //
    fprintf(py.fptr, "\"\"\"Generated by the Chapel compiler.\"\"\"\n");
    fprintf(py.fptr, "\n");
    fprintf(py.fptr, "import atexit\n");
    fprintf(py.fptr, "\n");
    fprintf(py.fptr, "from %s.%s import *\n", libDir, pythonModulename);
    fprintf(py.fptr, "\n");
    fprintf(py.fptr, "# Register cleanup function to be called at "
                     "program exit.\n");
    fprintf(py.fptr, "atexit.register(%s.chpl_cleanup)\n",
            pythonModulename);

    // Restore the previous file used for codegen.
    gGenInfo->cfile = save_cfile;
  }

  // Don't "beautify", it will remove the tabs.
  closeLibraryHelperFile(&py, false);

  return;
}

// Once all the python files have been generated and the .a/.so has been made,
// make the Python module!
void codegen_make_python_module() {
  const char* oldPath = getenv("PYTHONPATH");
  std::string pythonPath = "";
  if (oldPath == NULL) {
    pythonPath = CHPL_RUNTIME_INCL;
  } else {
    pythonPath += oldPath;
    pythonPath += ":";
    pythonPath += CHPL_RUNTIME_INCL;
  }
  pythonPath += "/python";

  // On Crays, we need to use the exact same C compiler and linker that Chapel
  // detects
  std::string crayCompiler;
  std::string crayLinker;
  if (CHPL_TARGET_COMPILER != NULL &&
      strstr(CHPL_TARGET_COMPILER, "cray-prgenv") != NULL) {
    if (strcmp(CHPL_TARGET_COMPILER, "cray-prgenv-gnu") != 0) {
      USR_WARN("PrgEnvs other than gnu may not be ABI compatible with Cython"
               " source files");
    }
    std::string getCrayComp = "$CHPL_HOME/util/config/compileline --compiler";
    crayCompiler = runCommand(getCrayComp);
    // Erase the trailing \n from getting the cFlags
    crayCompiler.erase(crayCompiler.length() - 1);
    std::string getCLink = "$CHPL_HOME/util/config/compileline --linkershared";
    crayLinker = runCommand(getCLink);
    crayLinker.erase(crayLinker.length() - 1);
  }

  std::string getCFlags = "$CHPL_HOME/util/config/compileline --cflags";
  std::string cFlags = runCommand(getCFlags);
  // Erase the trailing \n from getting the cFlags
  cFlags.erase(cFlags.length() - 1);
  std::string requireIncludes = getRequireIncludes();
  std::string getIncludes =
    "$CHPL_HOME/util/config/compileline --includes-and-defines";
  std::string includes = runCommand(getIncludes);
  // Erase the trailing \n from getting the includes
  includes.erase(includes.length() - 1);

  std::string requireLibraries = getRequireLibraries();
  std::string getLibraries = "$CHPL_HOME/util/config/compileline --libraries";
  std::string libraries = runCommand(getLibraries);
  // Erase the trailing \n from getting the libraries
  libraries.erase(libraries.length() - 1);

  // Snag extra dependencies for multilocale libraries if needed.
  if (fMultiLocaleInterop) {
    std::string cmd = "$CHPL_HOME/util/config/compileline";
    cmd += " --multilocale-lib-deps";
    libraries += " ";
    libraries += runCommand(cmd);
    libraries.erase(libraries.length() - 1);
  }

  std::string name = "-l";
  int libLength = strlen("lib");
  bool startsWithLib = strncmp(executableFilename, "lib", libLength) == 0;
  if (startsWithLib) {
    name += &executableFilename[libLength];
  } else {
    name += executableFilename;
  }

  std::string cythonPortion = "python3 ";
  cythonPortion += pythonModulename;
  cythonPortion += ".py build_ext -i";

  std::string fullCythonCall = "PYTHONPATH=" + pythonPath;
  if (crayCompiler != "") {
    fullCythonCall += " CC=\"" + crayCompiler;
    fullCythonCall += "\" LDSHARED=\"" + crayLinker + "\"";
  }
  fullCythonCall += " CFLAGS=\"" + cFlags + requireIncludes + " " + includes;
  fullCythonCall += "\" LDFLAGS=\"-L. " + name + requireLibraries;
  fullCythonCall += " " + libraries;

  // Append library as last link argument to appease GNU linker.
  fullCythonCall += " " + name;

  fullCythonCall +=  "\" " + cythonPortion;

  std::string chdirIn = "cd ";
  chdirIn += libDir;
  chdirIn += "; ";
  std::string fullCommand = chdirIn + fullCythonCall;
  runCommand(fullCommand);
}

// Skip this function if it is defined in an internal module, or if it is
// the generated main function
bool isUserRoutine(FnSymbol* fn) {
  return !(fn->getModule()->modTag == MOD_INTERNAL ||
           fn->getModule()->modTag == MOD_STANDARD ||
           fn->hasFlag(FLAG_GEN_MAIN_FUNC));
}
