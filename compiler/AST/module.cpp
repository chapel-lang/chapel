#include "files.h"
#include "module.h"
#include "stringutil.h"

Module* currentModule = NULL;


Module::Module(char* inFilename) :
  codefile(NULL),
  extheadfile(NULL),
  intheadfile(NULL),
  stmts(nilStmt)
{
  filename = copystring(inFilename);
  FILE* testfile = openInputFile(filename);
  closeInputFile(testfile);
}


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


Module* Module::createModules(int numFilenames, char* filename[]) {
  Module* modules = NULL;

  for (int i=0; i<numFilenames; i++) {
    Module* newModule = new Module(filename[i]);
    modules = appendLink(modules, newModule);
  }

  return modules;
}

