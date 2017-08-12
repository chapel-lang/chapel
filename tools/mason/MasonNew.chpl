
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
      if isDir(args[2]) {
	writeln('A directory with that name already exists');
      }
      else {
	InitProject(args);
      }
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
    halt("Mason could not create Library Project: " + name);
  }
  else {
    makeBasicToml(name);
    makeProjectFiles(name);
    writeln("Created Library Project: " + name);
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
  const libTemplate = '\n /* Documentation for ' + name +
    ' */\nmodule '+ name + ' { \nwriteln("New library: '+ name +'");\n}';
  var lib = open(name+'/src/'+name+'.chpl', iomode.cw);
  var libWriter = lib.writer();
  libWriter.write(libTemplate);
  libWriter.close();
}