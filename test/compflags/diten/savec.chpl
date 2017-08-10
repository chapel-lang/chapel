const outdir = "savec_output";
const filename = "savec.chpl";

var ret: int;

extern proc system(s: c_string): int(32);

ret = system((CHPL_HOME+"/bin/"+CHPL_HOST_PLATFORM+"/"+"chpl --savec " + outdir + " " + filename).c_str());
if ret != 0 then
  halt("Error compiling Chapel code");

ret = system(("make -f " + outdir + "/Makefile > /dev/null 2>&1").c_str());
if ret != 0 then
  halt("Error compiling C code");

ret = system(("rm -r " + outdir).c_str());
if ret != 0 then
  halt("Error removing savec directory");
