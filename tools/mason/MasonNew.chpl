
use Path;
use Spawn;
use FileSystem;
use MasonUtils;
use MasonHelp;


config const vcs: bool = true;

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


proc InitProject(args: [] string) {
  const name = args[2];
  var status = -1;
  if vcs {
    status = gitInit(name);
    addGitIgnore(name);
  }
  else {
    status = noGitInit(name);
  }

  if status != 0 {
    halt("Mason failed to initialize your project.");
  }
  else {
    makeBasicToml(name);
    makeProjectFiles(name);
    writeln("Mason initialized your new project: " + name);
  }
}


proc gitInit(name: string) : int {
  var command = "git init -q " + name;
  return runWithStatus(command);
}

proc addGitIgnore(name: string) {
  var toIgnore = "\ntarget/\nMason.lock";
  var gitIgnore = open(name+"/.gitignore", iomode.cw);
  var GIwriter = gitIgnore.writer();
  GIwriter.write(toIgnore);
  GIwriter.close();
}

proc noGitInit(name: string) : int {
  var command = "mkdir " + name;
  return runWithStatus(command);
}

proc makeBasicToml(name: string) {
  const baseToml = '\n[brick]\nname = "' + name +
    '"\nversion = "0.1.0"\n\n[dependencies]\n';
  var tomlFile = open(name+"/Mason.toml", iomode.cw);
  var tomlWriter = tomlFile.writer();
  tomlWriter.write(baseToml);
  tomlWriter.close();
}
  

proc makeProjectFiles(name: string) {
  runCommand("mkdir " + name + "/src");
  runCommand("touch " + name + "/src/" + name + '.chpl');
}