const filename = "hello.chpl";

proc mysystem(cmd: string): int {
  use Spawn;
  var sub = spawnshell(cmd);
  sub.wait();
  return sub.exit_status;
}

var binpath = CHPL_HOST_PLATFORM + "-" + CHPL_HOST_ARCH;
var ret = mysystem(CHPL_HOME + "/bin/" + binpath + "/" +
                   "chpl -o a.out " + filename);
if ret != 0 then
  halt("Error compiling Chapel code");

ret = mysystem("./a.out -nl 1 < test-dir");

ret = mysystem("rm a.out*");
if ret != 0 then
  halt("Error removing a.out executable(s)");
