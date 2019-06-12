use MasonTest;
use MasonUtils;
use Spawn;


proc checkExitStatus(cmd: string) {
  var splitCmd = cmd.split();
  var p = spawn(splitCmd,stdout=PIPE);
  p.wait();
  if p.exit_status == 1 {
    writeln("Got exit status 1 as expected");
  }
}

checkExitStatus('mason test');
 