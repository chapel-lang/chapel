use MasonBuild;
use MasonHelp;
use MasonUtils;
use FileSystem;
use TOML;

proc masonRun(args) {
  var toParse = open("Mason.toml", iomode.r);
  var tomlFile = parseToml(toParse);
  var project = tomlFile["brick"]["name"].s;
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
    var command = "target/debug/" + project + ' ' + execs;
    if isDir('target/release') then
      command = "target/release/" + project + ' ' + execs;

    if show then
      writeln("Executing binary: " + command);

    if isFile("Mason.lock") then  // If built
      runCommand(command);
    else if isFile("Mason.toml") { // If not built
      masonBuild(args);
      runCommand(command);
    }
    else writeln("call mason run from the top level of your projects directory");

    // Close memory
    delete tomlFile;
    toParse.close();
  }
  else {
    writeln("Mason cannot find the compiled program");
    exit();
  }
}

