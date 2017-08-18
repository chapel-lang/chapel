
/* A helper file of utilities for Mason */
use Spawn;
use FileSystem;

/* Gets envoirnment variables for spawn commands */
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
   downloaed previously */
proc depExists(dependency: string) {
  var repos = getMasonHome() +'/.mason/src/';
  var exists = false;
  for dir in listdir(repos) {
    if dir == dependency {
      exists = true;
    }
  }
  return exists;
}


proc getMasonHome(): string {
  var masonHome = getEnv("CHPL_MASON_HOME");
  if masonHome == '' {
    var home = getEnv('HOME');
    if isFile(home + '/.mason') {
      return home;
    }
    else {
      writeln("Mason could not find CHPL_MASON_HOME");
      writeln("Consider setting CHPL_MASON_HOME in your .bashrc");
      halt();
    }
  }
  else {
    return masonHome;
  }
}