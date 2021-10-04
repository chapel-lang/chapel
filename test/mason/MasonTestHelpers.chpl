use Spawn;

proc checkExitStatus(cmd, exitStatus) {
  var p = spawn(cmd, stdout=PIPE);
  p.wait();
  if p.exitCode == exitStatus {
    writeln("Got exit status %i as expected".format(exitStatus));
  }
}
