use MasonTest;
use MasonUtils;
use Subprocess;


proc checkExitStatus(cmd) {
  var p = spawn(cmd, stdout=PIPE);
  p.wait();
  if p.exitCode == 1 {
    writeln("Got exit status 1 as expected");
  }
}

checkExitStatus(['mason','test']);
