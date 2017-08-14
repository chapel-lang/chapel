
use MasonNew;
use MasonBuild;
use MasonUpdate;
use MasonUtils;
use MasonHelp;
use FileSystem;


proc main(args: [] string) {
  if args.size < 2 {
    masonHelp();
    exit();}

  select (args[1]) {
  when 'new' do masonNew(args);
  when 'build' do masonBuild(args);
  when 'update' do UpdateLock();
  when 'run' do masonRun(args);
  when 'clean' do masonClean();
  when '--list' do masonList();
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


config const build: bool = false;

/* TODO: mtime so compilation only when nessescary
   TODO: eliminate need for main name  */
proc masonRun(args) {
  var toRun = basename(getEnv('PWD'));
  if build then masonBuild(args);
  if args.size > 2 {
    select (args[2]) {
    when '-h' do masonRunHelp();
    when '--help' do masonRunHelp();
    otherwise {
      writeln("Invalid run argument");
      writeln("Try mason run --help");
      exit();
    }
    }
  }
  else {
    if isDir('target') {
      if isFile("Mason.lock") {
	var command = "target/debug/" + toRun;
	runCommand(command);
      }
      else if isFile("Mason.toml") {
	masonBuild(args);
	var command = "target/debug/" + toRun;
	runCommand(command);
      }
      else {
	writeln("call mason run from the top level of your projects directory");
      }
    }
    else {
      writeln("Mason cannot find the compiled program");
      writeln("Try mason run --build");
      exit();
    }
  }
}


proc masonClean() {
  runCommand('rm -rf target');
}



// TODO
proc masonInit(args) {}
proc masonDoc(args) {}


proc printVersion() {
  writeln('mason 0.0.1 (SHA <date>)');
}

