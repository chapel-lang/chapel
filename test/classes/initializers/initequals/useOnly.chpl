use Spawn only spawnshell, PIPE, subprocess;
use IO;

proc main() throws {
  var p: subprocess(kind=iokind.dynamic, locking=true);
  p = run('ls');
}
proc run(cmd: string): subprocess(kind=iokind.dynamic, locking=true) throws {
  var p = spawnshell(cmd, stdout=PIPE, stderr=PIPE);
  return p;
}
