
/* A helper file of utilities for Mason */
use Spawn;
use FileSystem;

/* Gets environment variables for spawn commands */
extern proc getenv(name : c_string) : c_string;
proc getEnv(name: string): string {
  var cname: c_string = name.c_str();
  var value = getenv(cname);
  return value:string;
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


/* Same as runCommand but for situations where an
   exit status is needed */
proc runWithStatus(command): int {
  var cmd = command.split();
  var sub = spawn(cmd, stdout=PIPE);

  var line:string;
  while sub.stdout.readline(line) do write(line);
  sub.wait();
  return sub.exit_status;
}



/* Checks to see if dependency has already been
   downloaded previously */
proc depExists(dependency: string) {
  var repos = MASON_HOME +'/.mason/src/';
  var exists = false;
  for dir in listdir(repos) {
    if dir == dependency then
      exists = true;
  }
  return exists;
}


proc MASON_HOME: string {
  // possible locations
  var masonHome = getEnv("MASON_HOME");
  var home = getEnv('HOME');
  var testDir = getEnv('PWD');
  if masonHome == '' {
    if isDir(home + '/.mason') then
      return home;
    // Fix for testing
    else if isDir(testDir + '/.mason') {
      return testDir;
    }
    else {
      writeln("Mason could not find MASON_HOME");
      writeln("Consider setting MASON_HOME in your .bashrc");
      halt();
    }
  }
  else return masonHome;
}