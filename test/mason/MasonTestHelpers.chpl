use Spawn;

proc checkExitStatus(cmd: [] string, exitStatus: int) {
  var p = spawn(cmd, stdout=PIPE);
  p.wait();
  if p.exitCode == exitStatus {
    writeln("Got exit status %i as expected".format(exitStatus));
  } else {
    writeln("Error: %s returned %i when test expected %i".format(" ".join(cmd),
                                                                 p.exitCode,
                                                                 exitStatus));
  }
}
