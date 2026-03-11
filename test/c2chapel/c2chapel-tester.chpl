import Subprocess, ChplConfig.CHPL_HOME;

var testdir = CHPL_HOME + "/tools/c2chapel/test/";
var command = "%s/tester.sh".format(testdir);
var sub = Subprocess.spawn([command]);
sub.communicate();
if sub.exitCode==0 then
  writeln("OK");
else
  writeln("Test Failure");
