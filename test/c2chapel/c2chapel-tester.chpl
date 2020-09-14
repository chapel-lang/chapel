import Spawn;

var testdir = CHPL_HOME + "/tools/c2chapel/test/";
var command = "cd %s; ./tester.sh".format(testdir);
var sub = Spawn.spawnshell(command);
sub.communicate();
if sub.exit_status==0 then
  writeln("OK");
else
  writeln("Test Failure");
