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
#include "expr.h"
#include "stlUtil.h"
#include "stringutil.h"

char libDir[FILENAME_MAX + 1]  = "";

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
            fn->getModule()->modTag != MOD_INTERNAL &&
            fn->hasFlag(FLAG_GEN_MAIN_FUNC) == false) {
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
  replace += "--fixpath '$(CHPL_HOME)' $CHPL_HOME < cmd.out.tmp";

  std::string res = runCommand(replace);
  std::string cleanup = "rm cmd.out.tmp";
  runCommand(cleanup);
  return res;
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
  fprintf(makefile.fptr, "CHPL_LDFLAGS = -L%s %s %s \n",
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

  // TODO: indentation is lost.  Probably from cleanup when file is closed.
  // Figure out what to do about it, it will be necessary because python
  fprintf(pxd, "cdef extern from \"chpltypes.h\":\n");
  fprintf(pxd, "\tctypedef void* c_fn_ptr\n\n");

  fprintf(pxd, "cdef extern from \"chpl-init.h\":\n");
  fprintf(pxd, "\tvoid chpl_library_init(int argc, char* argv[])\n");
  fprintf(pxd, "\tvoid chpl_library_finalize()\n");
}

void codegen_library_python(std::vector<FnSymbol*> functions) {
  if (fLibraryCompile && fLibraryPython) {
    fileinfo pxd = { NULL, NULL, NULL };

    openLibraryHelperFile(&pxd, libmodeHeadername, "pxd");

    if (pxd.fptr != NULL) {
      FILE* save_cfile = gGenInfo->cfile;

      gGenInfo->cfile = pxd.fptr;

      fprintf(pxd.fptr, "cdef extern from \"%s.h\":\n", libmodeHeadername);

      for_vector(FnSymbol, fn, functions) {
        if (fn->getModule()->modTag != MOD_INTERNAL &&
            fn->hasFlag(FLAG_GEN_MAIN_FUNC) == false) {
          // TODO: make that check a function?
          fn->codegenPXD();
        }
      }

      pxdEnd();
      gGenInfo->cfile = save_cfile;
    }
    closeLibraryHelperFile(&pxd);
  }
}

void FnSymbol::codegenPXD() {
  GenInfo *info = gGenInfo;

  if (!hasFlag(FLAG_EXPORT)) return;
  if (hasFlag(FLAG_NO_PROTOTYPE)) return;
  if (hasFlag(FLAG_NO_CODEGEN)) return;

  // Should I add the break-on-codegen-id stuff here?

  if (info->cfile) {
    FILE* outfile = info->cfile;
    if (fGenIDS)
      fprintf(outfile, "%s", idCommentTemp(this));

    fprintf(outfile, "%s;\n", codegenPXDType().c.c_str());
  } else {
    // TODO: LLVM stuff
  }
}

GenRet FnSymbol::codegenPXDType() {
  GenInfo* info = gGenInfo;
  GenRet ret;

  ret.chplType = typeInfo();

  if (info->cfile) {
    // Cast to right function type.
    std::string str;

    std::string retString = ""/* TODO: get return type here */;
    str += retString.c_str();
    str += " ";
    str += cname;
    str += "(";

    if (numFormals() != 0) {
      int count = 0;
      for_formals(formal, this) {
        if (formal->hasFlag(FLAG_NO_CODEGEN))
          continue; // do not print locale argument, end count, dummy class
        if (count > 0)
          str += ",\n";
        str += ""/* TODO: formal type */;
        str += " ";
        str += formal->cname;
        if (fGenIDS) {
          str += " ";
          str += idCommentTemp(formal);
        }
        count++;
      }
    } // pxd files do not take void as an argument list, just close the parens
    str += ")";
    ret.c = str;

  } else {
    // TODO: LLVM stuff
  }

  return ret;
}
