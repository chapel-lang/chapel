
use FileSystem;
use Help;
use Spawn;
use TOML;
use MasonNew;
use MasonBuild;
use MasonUpdate;

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
    exit();}
  }
}


proc masonNew(args) {
  if args.size < 3 {
    writeln('error: Invalid arguments.');
    masonNewHelp();
    exit();
  }
  else {
    select (args[2]) {
    when '--help' do masonNewHelp();
    when '-h' do masonNewHelp();
    otherwise {
      InitProject(args);
    }
    }
  }
}



proc masonBuild(args) {
  if args.size > 2 {
    select (args[2]) {
    when '-h' do masonBuildHelp();
    when '--help' do masonBuildHelp();
    }
  }
  UpdateLock();
  BuildProgram();
}

/* TODO: mtime so compilation only when nessescary
   TODO: eliminate need for main name  */
proc masonRun(args) {
  masonBuild(args);  
  if args.size < 3 {
    writeln("Cannot run: lacking main file name"); 
  }
  else {
    runCommand("target/debug/" + args[2]);
  }
}




proc masonInit(args) {}
proc masonClean(args) {}
proc masonDoc(args) {}

proc runCommand(cmd) {
   var splitCmd = cmd.split();
   var process = spawn(splitCmd, stdout=PIPE);
   process.wait();
   
   for line in process.stdout.lines() {
     writeln(line);
   }
}

/* Gets envoirment variables for spawn commands */
extern proc getenv(name : c_string) : c_string;
proc getEnv(name: string): string {
  var cname: c_string = name.c_str();
  var value = getenv(cname);
  return value:string;
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
  writeln('    new         Create a new mason project');
  writeln('    update      Update/Generate Mason.lock');
  writeln('    build       Compile the current project');
  //writeln('    clean       Remove the target directory');
  //writeln('    doc         Build this project\'s and its dependencies\' documentation');
  //writeln('    init        Create a new mason project in an existing directory');
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
  writeln('   - Build from mason project if Mason.lock present');
}

proc masonNewHelp() {
  writeln('Usage:');
  writeln('    mason new [options] <project name>');
  writeln('    mason new -h | --help');
}


proc printVersion() {
  writeln('mason 0.0.1 (SHA <date>)');
}

