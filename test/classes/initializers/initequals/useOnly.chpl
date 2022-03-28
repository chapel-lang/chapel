use Subprocess only spawnshell, pipeStyle, subprocess;
use IO;

proc main() throws {
  var p: subprocess(kind=iokind.dynamic, locking=true);
  p = run('ls');
}
proc run(cmd: string): subprocess(kind=iokind.dynamic, locking=true) throws {
  var p = spawnshell(cmd, stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);
  return p;
}
