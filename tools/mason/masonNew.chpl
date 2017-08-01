
use Path;
use TOML;
use Spawn;
use FileSystem;

proc main(args: [] string) {
  const name = args[1]; // change main later
  var status = -1;
  status = gitInit(name);
  if status != 0 {
    halt("Mason new failed");
  }
  else {
    makeBasicToml(name);
    makeProjFiles(name);
    writeln("Mason Initialized your new project: " + name);
  }
}


proc gitInit(name: string) : int {
  var command = "git init " + name;
  return runCommand(command);
}


proc makeBasicToml(name: string) {
  const baseToml = '\n[brick]\nname = "' + name +
    '"\nversion = "0.1.0"\n\n[dependencies]\n';
  var tomlFile = open("Mason.toml", iomode.cw);
  var tomlWriter = tomlFile.writer();
  tomlWriter.write(baseToml);
  tomlWriter.close();
}
  

proc makeProjFiles(name: string) {
  var makeSrc = spawnshell("mkdir src");
  makeSrc.wait();
  var makeLib = spawnshell("touch src/lib.chpl");
  makeLib.wait();
  moveFile(name, "src");
  moveFile(name, "Mason.toml");
}


  proc moveFile(name: string, file: string) {
  var dir = realPath(name);
  var command = "mv " + file + ' ' + dir;
  var result = spawnshell(command);
  result.wait();
}


proc runCommand(command): int {
  var cmd = command.split();
  var sub = spawn(cmd, stdout=PIPE);

  var line:string;
  while sub.stdout.readline(line) do write(line);
  sub.wait();
  return sub.exit_status;
}