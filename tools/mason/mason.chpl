
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




proc masonRun(args) {
  var toRun = basename(getEnv('PWD'));
  var show = false;
  var execopts: [1..0] string;
  if args.size > 2 {
    for arg in args[2..] {
      if arg == '-h' || arg == '--help' {
        masonRunHelp();
	exit();
      }
      else if arg == '--build' {
	masonBuild(['mason', 'build']);
      }
      else if arg == '--show' {
	show = true;
      }
      else {
	execopts.push_back(arg);
      }
    }  
  }
  // Find the Binary and execute 
  if isDir('target') {
    var execs = ' '.join(execopts);
    var command = "target/debug/" + toRun + ' ' + execs;
    if isDir('target/release') {
      command = "target/release/" + toRun + ' ' + execs;
    }
    if show {
      writeln("Executing binary: " + command);
    }
    if isFile("Mason.lock") {  // If built
      runCommand(command);
    }
    else if isFile("Mason.toml") { // If not built
      masonBuild(args);
      runCommand(command);
    }
    else {
      writeln("call mason run from the top level of your projects directory");
    }
  }
  else {
    writeln("Mason cannot find the compiled program");
    exit();
  }
}


proc masonClean() {
  runCommand('rm -rf target');
}



// TODO
proc masonInit(args) {}
proc masonDoc(args) {}


proc printVersion() {
  writeln('mason 0.1.0');
}

