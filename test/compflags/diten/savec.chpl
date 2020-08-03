const outdir = "savec_output";
const filename = "savec.chpl";

proc mysystem(cmd: string): int {
  use Spawn;
  var sub = spawnshell(cmd);
  sub.wait();
  return sub.exit_status;
}

var binpath = CHPL_HOST_PLATFORM + "-" + CHPL_HOST_ARCH;
var ret = mysystem(CHPL_HOME + "/bin/" + binpath + "/" +
                   "chpl -o a.out --no-llvm --savec " + outdir + " " + filename);
if ret != 0 then
  halt("Error compiling Chapel code");

ret = mysystem("rm a.out*");
if ret != 0 then
  halt("Error removing a.out executable(s)");

ret = mysystem("make -f " + outdir + "/Makefile > /dev/null 2>&1");
if ret != 0 then
  halt("Error compiling C code");

ret = mysystem("rm a.out*");
if ret != 0 then
  halt("Error removing a.out executable(s)");

ret = mysystem("rm -r " + outdir);
if ret != 0 then
  halt("Error removing savec directory");
