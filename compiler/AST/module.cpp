#include "files.h"
#include "module.h"

Module* currentModule = NULL;

Module::Module(void) :
  codefile(NULL),
  extheadfile(NULL),
  intheadfile(NULL),
  stmts(nilStmt)
{}


static void genHeader(FILE* outfile, char* extheadfilename, 
		      char* intheadfilename) {
  fprintf(outfile, "#include \"stdchpl.h\"\n");
  fprintf(outfile, "#include \"%s\"\n", extheadfilename);
  fprintf(outfile, "#include \"%s\"\n", intheadfilename);
  fprintf(outfile, "\n");
}


void Module::codegen(void) {
  fileinfo outfileinfo;
  fileinfo extheadfileinfo;
  fileinfo intheadfileinfo;

  openCFiles(filename, &outfileinfo, &extheadfileinfo, &intheadfileinfo);
  currentModule->codefile = outfileinfo.fptr;
  currentModule->extheadfile = extheadfileinfo.fptr;
  currentModule->intheadfile = intheadfileinfo.fptr;

  genHeader(currentModule->codefile, extheadfileinfo.filename, 
	    intheadfileinfo.filename);

  stmts->codegenList(currentModule->codefile, "\n");

  closeCFiles(&outfileinfo, &extheadfileinfo, &intheadfileinfo);
}
