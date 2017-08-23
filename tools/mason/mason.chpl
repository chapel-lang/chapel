
use MasonNew;
use MasonBuild;
use MasonUpdate;
use MasonUtils;
use MasonHelp;
use FileSystem;

/*

The Design of Mason
===================

  Mason is a command line tool for building chapel programs to provide users with
       a consistent way of building applications and libraries. Mason uses
       a four state pipeline to go from start to finish in a project. The
       four states are listed below.


  Four States:

  1) Project Code: ``yourProject/src/yourProject.chpl``
           This is the source code of the project the user creates using mason.
  2) Manifest File: ``Mason.toml``
           Toml file containing metadata and dependencies
           Builds dependency directed acyclic graph (DAG) to be
           serialized into lock file
  3) Lock File:  ``Mason.lock``
           Contains necessary build information
           Serialized directed acyclic graph of the dependencies build options
           from the manifest
  4) Dependency Code:  ``$MASON_HOME/.mason/src``
           Local dependencies downloaded by mason after the user lists them in
           a project manifest.

Full documentation is located in the chapel release in $CHPL_HOME/doc/rst/tools/mason/mason.rst

*/



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
    if isDir('target/release') then
      command = "target/release/" + toRun + ' ' + execs;

    if show then
      writeln("Executing binary: " + command);

    if isFile("Mason.lock") then  // If built
      runCommand(command);
    else if isFile("Mason.toml") { // If not built
      masonBuild(args);
      runCommand(command);
    }
    else writeln("call mason run from the top level of your projects directory");
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

