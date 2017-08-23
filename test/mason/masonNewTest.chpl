
use Spawn;
use FileSystem;


proc main() {
  writeln(masonNewTest());
  runCommand('rm -rf Test');
}

proc masonNewTest() {
  runCommand("mason new Test");
  if isFile('Test/Mason.toml') {
    if isFile('Test/src/Test.chpl') {
      return true;
    }
  }
  return false;
}


/* Uses the Spawn module to create a subprocess */
proc runCommand(cmd) {
  var splitCmd = cmd.split();
  var process = spawn(splitCmd, stdout=PIPE);
  process.wait();

  for line in process.stdout.lines() {
    write(line);
  }
}