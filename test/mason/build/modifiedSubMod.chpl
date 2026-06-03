use FileSystem, Path, IO, Time;
use MasonBuild;

proc main() {
  const package = '_subModule';
  here.chdir(package);
  const args = ["build"];
  masonBuild(args);

  sleep(2); // ensure mod time difference

  // edit _subModule/src/_subModule/Bar.chpl so that it has a syntax error
  // since we modified the file, it should trigger a rebuild and fail
  const barPath = joinPath("src", "_subModule", "Bar.chpl");
  const backup = "myBackup.bak";
  copy(barPath, backup);
  defer {
    copy(backup, barPath);
    remove(backup);
  }

  var barFile = open(barPath, ioMode.a);
  var writer = barFile.writer();
  writer.writeln("this is a syntax error");
  writer.close();
  barFile.close();

  writeln("Modified Bar.chpl to introduce syntax error. rebuilding...\n");

  try {
    masonBuild(args);
    writeln("Expected masonBuild to throw an error due to syntax error, but it did not.");
  } catch e {
    writeln("Caught expected error from masonBuild due to syntax error:");
    writeln(e);
  }
}
