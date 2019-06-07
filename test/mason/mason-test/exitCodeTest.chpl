use MasonTest;
use MasonUtils;
use Spawn;


proc checkExitStatus(cmd: string) {
  var splitCmd = 'mason test'.split();
  var p = spawn(splitCmd);
  p.wait();
  if p.exit_status == 1 {
    exit(0);
  }

  checkCommand('mason test');
}
 