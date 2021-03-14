use MasonUtils;
use MasonHelp;
use Spawn;

proc checkCommand(cmd: string) {
  var splitCmd = cmd.split();
  var p = spawn(splitCmd,stdout=PIPE);
  p.wait();
  if p.exit_status != 0 {
    writeln("Failed to run cmd: '%s'".format(cmd));
  }
}

checkCommand('mason new -h');
checkCommand('mason init -h');
checkCommand('mason add -h');
checkCommand('mason rm -h');
checkCommand('mason build -h');
checkCommand('mason run -h');
checkCommand('mason search -h');
checkCommand('mason env -h');
checkCommand('mason clean -h');
checkCommand('mason doc -h');
checkCommand('mason system -h');
checkCommand('mason test -h');
checkCommand('mason external -h');
