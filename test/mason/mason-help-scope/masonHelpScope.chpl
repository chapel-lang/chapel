
use MasonUtils;
use FileSystem;
use MasonHelp;

proc main() {
  runCommand('mason new -h');
  runCommand('mason add -h');
  runCommand('mason rm -h');
  runCommand('mason build -h');
  runCommand('mason run -h');
  runCommand('mason search -h');
  runCommand('mason env -h');
  runCommand('mason clean -h');
  runCommand('mason doc -h');
  runCommand('mason system -h');
  runCommand('mason  test -h');
  runCommand('mason external -h');
  exit(0);
  
}
