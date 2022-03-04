use Subprocess;

proc checkExitStatus(cmd: [] string, exitStatus: int) {
  var p = spawn(cmd, stdout=pipeStyle.pipe);
  var cmdString = " ".join(cmd);
  p.wait();
  if p.exitCode == exitStatus {
    writef("Got exit status %i as expected\n", exitStatus);
  } else {
    writef("Error: %s returned %i when test expected %i\n", cmdString,
                                                            p.exitCode,
                                                            exitStatus);
  }
}
