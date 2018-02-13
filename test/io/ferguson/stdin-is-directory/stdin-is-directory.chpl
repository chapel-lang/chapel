const filename = "hello.chpl";

proc mysystem(cmd: string): int {
  use Spawn;
  var sub = spawnshell(cmd);
  sub.wait();
  return sub.exit_status;
}

var ret = mysystem(CHPL_HOME + "/bin/" + CHPL_HOST_PLATFORM + "/" +
                   "chpl -o a.out " + filename);
if ret != 0 then
  halt("Error compiling Chapel code");

ret = mysystem("./a.out -nl 1 < test-dir");

ret = mysystem("rm a.out*");
if ret != 0 then
  halt("Error removing a.out executable(s)");
