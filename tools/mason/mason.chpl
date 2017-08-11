
use MasonNew;
use MasonBuild;
use MasonUpdate;
use MasonUtils;
use MasonHelp;

proc main(args: [] string) {
  if args.size < 2 {
    masonHelp();
    exit();}

  select (args[1]) {
  when 'new' do masonNew(args);
  when 'build' do masonBuild(args);
  when 'update' do UpdateLock();
  when 'run' do masonRun(args);
  when '-h' do masonHelp();
  when '--help' do masonHelp();
  when '-V' do printVersion();
  when '--version' do printVersion();
  otherwise {
    writeln('error: no such subcommand');
    writeln('try mason --help');
    exit();
  }
  }
}


/* TODO: mtime so compilation only when nessescary
   TODO: eliminate need for main name  */
proc masonRun(args) {
  var toRun = basename(getEnv('PWD'));
  if args.size > 2 {
    if args[2] == "build" {
      masonBuild(args);
    }
  }
  if isFile("Mason.lock") {
    var command = "target/debug/" + toRun;
    runCommand(command);
  }
  else {
    writeln("call mason run from the top level of your projects directory");
  }
}

// TODO
proc masonInit(args) {}
proc masonClean(args) {}
proc masonDoc(args) {}


proc printVersion() {
  writeln('mason 0.0.1 (SHA <date>)');
}

