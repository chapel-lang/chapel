#include "codegen.h"
#include "files.h"
#include "nils.h"

FILE* intheadfile;
FILE* extheadfile;

static void genHeader(FILE* outfile, char* extheadfilename, 
		      char* intheadfilename) {
  fprintf(outfile, "#include \"stdchpl.h\"\n");
  fprintf(outfile, "#include \"%s\"\n", extheadfilename);
  fprintf(outfile, "#include \"%s\"\n", intheadfilename);
  fprintf(outfile, "\n");
}


void codegen(char* infilename, char* compilerDir, Stmt* program) {
  fileinfo outfileinfo;
  fileinfo extheadfileinfo;
  fileinfo intheadfileinfo;

  openMakefile(infilename, compilerDir);
  openCFiles(infilename, &outfileinfo, &extheadfileinfo, &intheadfileinfo);

  FILE* outfile = outfileinfo.fptr;
  extheadfile = extheadfileinfo.fptr;
  intheadfile = intheadfileinfo.fptr;

  genHeader(outfile, extheadfileinfo.filename, intheadfileinfo.filename);
  program->codegenList(outfile, "\n");

  verifyNilsUncorrupted();

  closeCFiles(&outfileinfo, &extheadfileinfo, &intheadfileinfo);
  closeMakefile();

  makeAndCopyBinary();
}
