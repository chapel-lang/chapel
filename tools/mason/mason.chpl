
use FileSystem;
use Help;
use Spawn;
use TOML;


proc main(args: [] string) {
  if args.size < 2 {
    masonHelp();
    exit();}

  select (args[1]) {
  when 'build' do masonBuild(args);
  when 'run' do masonRun(args);
  when '-h' do masonHelp();
  when '--help' do masonHelp();
  when '-V' do printVersion();
  when '--version' do printVersion();
  otherwise {
    writeln('error: no such subcommand');
    writeln('try mason --help');
    exit();}
  }
}


proc masonBuild(args) {
  if args.size < 2 {
    writeln('error: Invalid arguments.');
    masonNewHelp();
    exit();
  } 
  else if args.size > 2 {
    select (args[2]) {
    when '-h' do masonBuildHelp();
    when '--help' do masonBuildHelp();
    }
    var command = '';
    var status = -1;
    if isFile('Mason.toml') {
      writeln("found it");
    } 
    else {
      halt('No Mason.toml');
    }

  if status != 0 then
    halt('Error: Build failed');
  }

}


proc masonInit(args) {}
proc masonClean(args) {}
proc masonDoc(args) {} //chpldoc


proc masonNew(args) {
  if args.size < 3 {
    writeln('error: Invalid arguments.');
    masonNewHelp();
    exit();
  }
  else {
    var project = args[2]; // create project structure
  }
}


proc masonRun(args) {
  var command = '';
  var status = -1;
  if isFile('Mason.toml') {
    // Parse manifest file for flags, and build
    halt('Mason.toml not yet supported...');
  } else {
    masonBuild(args);
  }
}


proc runCommand(command): int {
  var cmd = command.split();
  var sub = spawn(cmd, stdout=PIPE);

  writeln(command);
  var line:string;
  while sub.stdout.readline(line) do write(line);
  sub.wait();
  return sub.exit_status;
}

/* Find Chapel program to serve as target for compilation */
proc findTarget() {
  for f in listdir() {
    if splitExt(f)(2) == 'chpl' then return f;
  }
  return '';
}


proc splitExt(s: string): (string, string) {
  const sp = s.split('.');
  if sp.size == 1 then
    return (s, '');
  else {
    var ext = sp[sp.size];
    var root = '.'.join(sp[..sp.size-1]);
    return (root, ext);
  }
}


proc masonHelp() {
  writeln("Chapel's package manager");
  writeln();
  writeln('Usage:');
  writeln('    mason <command> [<args>...]');
  writeln('    mason [options]');
  writeln();
  writeln('Options:');
  writeln('    -h, --help          Display this message');
  writeln('    -V, --version       Print version info and exit');
  writeln('    --list              List installed commands');
  writeln();
  writeln('Some common mason commands are (see all commands with --list):');
  writeln('    build       Compile the current project');
  writeln('    clean       Remove the target directory');
  writeln('    doc         Build this project\'s and its dependencies\' documentation');
  writeln('    new         Create a new mason project');
  writeln('    init        Create a new mason project in an existing directory');
  writeln('    run         Build and execute src/main.chpl');
}


proc masonBuildHelp() {
  writeln('Compile a local package and all of its dependencies');
  writeln();
  writeln('Usage:');
  writeln('    mason build [options]');
  writeln();
  writeln('Options:');
  writeln('    -h, --help                   Display this message');
  writeln();
  writeln('When no options are provided, the following will take place:');
  writeln('   - Build from mason project if Mason.toml present');
  writeln('   - Build from makefile if makefile present');
}

proc masonNewHelp() {
  writeln('Usage:');
  writeln('    mason new [options] <path>');
  //writeln('    mason new -h | --help');
}


proc printVersion() {
  writeln('mason 0.0.1 (SHA <date>)');
}

