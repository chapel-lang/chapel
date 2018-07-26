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

#include <string>

#include "FnSymbol.h"
#include "beautify.h"
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "stlUtil.h"
#include "stringutil.h"

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
    openCFile(&libhdrfile, libmodeHeadername, "h");
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
            fn->getModule()->modTag != MOD_INTERNAL &&
            fn->hasFlag(FLAG_GEN_MAIN_FUNC) == false) {
          fn->codegenPrototype();
        }
      }

      gGenInfo->cfile = save_cfile;
    }
    closeCFile(&libhdrfile);
  }
}

// Helper function to avoid unnecessary repetition when getting information
// from compileline
static std::string getCompilelineOption(std::string option) {
  std::string fullCommand = "$CHPL_HOME/util/config/compileline --" + option;
  fullCommand += "> cmd.out.tmp";
  runCommand(fullCommand);
  std::string replace = "$CHPL_HOME/util/config/replace-paths.py ";
  replace += "--fixpath '$(CHPL_HOME)' $CHPL_HOME < cmd.out.tmp";

  std::string res = runCommand(replace);
  std::string cleanup = "rm cmd.out.tmp";
  runCommand(cleanup);
  return res;
}

static void openLibraryHelperFile(fileinfo* fi,
                                  const char* name,
                                  const char* ext = NULL,
                                  bool useTmpDir = true);
static void closeLibraryHelperFile(fileinfo* fi, bool beautifyIt = true);

void codegen_library_makefile() {
  fileinfo makefile;
  // TODO: alter location to use generated library directory
  openLibraryHelperFile(&makefile, "Makefile.chpl_lib", "", false);

  // Save the CHPL_HOME location so it can be used in the other makefile
  // variables instead of letting them be cluttered with its value
  std::string cmd = "echo $CHPL_HOME";
  std::string chplHome = runCommand(cmd);
  fprintf(makefile.fptr, "CHPL_HOME = %s\n", chplHome.c_str());

  // TODO: compileline --includes-and-defines adds -I. to the list
  // automatically.  If the library is in a different directory from the
  // Makefile that is using this one, that won't be sufficient to find the
  // header file.  I will handle this with the output directory change
  std::string cflags = getCompilelineOption("cflags");
  cflags.erase(cflags.length() - 1); // remove trailing newline
  std::string includes = getCompilelineOption("includes-and-defines");
  fprintf(makefile.fptr, "CHPL_CFLAGS = %s %s\n",
          cflags.c_str(),
          includes.c_str());

  std::string libraries = getCompilelineOption("libraries");
  std::string libname = "-l";
  int libLength = strlen("lib");
  if (strncmp(executableFilename, "lib", libLength) == 0) {
    // libname = "-l<name>" when executableFilename = "lib<name>"
    libname += &executableFilename[libLength];
  } else {
    // libname = executableFilename plus the extension when executableFilename
    // does not start with "lib"
    libname = executableFilename;
    libname += getExtension();
  }
  // TODO: adjust for different location for the library, see earlier TODO
  fprintf(makefile.fptr, "CHPL_LDFLAGS = -L. %s %s \n",
          libname.c_str(),
          libraries.c_str());

  std::string compiler = getCompilelineOption("compiler");
  fprintf(makefile.fptr, "CHPL_COMPILER = %s\n", compiler.c_str());

  std::string linker = getCompilelineOption("linker");
  fprintf(makefile.fptr, "CHPL_LINKER = %s\n", linker.c_str());

  std::string linkerShared = getCompilelineOption("linkershared");
  fprintf(makefile.fptr, "CHPL_LINKERSHARED = %s", linkerShared.c_str());

  closeLibraryHelperFile(&makefile);
}

const char* getExtension() {
  if (fLibraryCompile) {
    if (fLinkStyle==LS_DYNAMIC) return ".so";
    else return ".a";
  }
  return "";
}

void openLibraryHelperFile(fileinfo* fi, const char* name, const char* ext,
                           bool useTmpDir) {
  if (ext)
    fi->filename = astr(name, ".", ext);
  else
    fi->filename = astr(name);

  if (useTmpDir) {
    fi->pathname = genIntermediateFilename(fi->filename);
  } else {
    fi->pathname = astr(name);
  }
  openfile(fi, "w");
}

void closeLibraryHelperFile(fileinfo* fi, bool beautifyIt) {
  closefile(fi->fptr);
  //
  // We should beautify if (1) we were asked to and (2) either (a) we
  // were asked to save the C code or (b) we were asked to codegen cpp
  // #line information (note that this can affect the output in the
  // event of a failed C compilation whether or not the --savec option
  // is used because a C codegen error will report the Chapel line #,
  // which can be helpful.
  //
  // TODO: With some refactoring, we could simply do the #line part of
  // beautify without also improving indentation and such which could
  // save some time.
  //
  if (beautifyIt && (saveCDir[0] || printCppLineno))
    beautify(fi);
}
