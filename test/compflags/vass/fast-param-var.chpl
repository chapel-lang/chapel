
if fastCompilation then
  compilerWarning("compiling with --fast");
else
  compilerWarning("compiling without --fast");

// no need to continue compilation
compilerError("done");
