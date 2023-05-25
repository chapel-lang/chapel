use MasonExternal;
use Subprocess;

// Test the output of all mason [cmd] -h

proc main() {
  checkOutput('mason new -h');
  checkOutput('mason init -h');
  checkOutput('mason add -h');
  checkOutput('mason rm -h');
  checkOutput('mason build -h');
  checkOutput('mason run -h');
  checkOutput('mason search -h');
  checkOutput('mason env -h');
  checkOutput('mason clean -h');
  checkOutput('mason doc -h');
  checkOutput('mason system -h');
  checkOutput('mason system pc -h');
  checkOutput('mason system search -h');
  checkOutput('mason test -h');
  checkOutput('mason external -h');
  // following commands require spack
  setupSpack();
  checkOutput('mason external search -h');
  checkOutput('mason external compiler -h');
  checkOutput('mason external install -h');
  checkOutput('mason external uninstall -h');
  checkOutput('mason external info -h');
}

proc checkOutput(cmd: string) {
  var splitCmd = cmd.split();
  var p = spawn(splitCmd,stdout=pipeStyle.pipe);
  writeln("$ " + cmd);
  var line:string;
  while p.stdout.readLine(line) {
    write(line);
  }
  writeln("*"*80);
  p.wait();
  if p.exitCode != 0 {
    writeln("Failed to run cmd: '%s'".format(cmd));
  }
}
