
use TOML;
use Spawn;
use FileSystem;
use masonInstall;

proc main(args: [] string) {
  if isFile("Mason.lock") {
    if !isDir("target") {
      // Make Target Directory
      makeTarget();
      makeTargetFiles();
    }

    //Install dependencies into .mason/src
    var toParse = open(args[1], iomode.r);
    var lockFile = parseToml(toParse);
    installDeps(lockFile);

    // Compile Program

    // Close memory
     delete lockFile;
     toParse.close();
  }
  else {
    writeln("Mason needs a lock file!");
  }
} 

proc makeTarget() {
  var makeTarget = "mkdir target";
  var process = spawnshell(makeTarget);
  process.wait();
}

// add if release turn optimizations on
proc makeTargetFiles() {
  var makeDebug  = "mkdir target/debug";
  var process = spawnshell(makeDebug);
  process.wait();

  var makeDeps = "mkdir target/debug/deps";
  var makeExamples = "mkdir target/debug/examples";
  var process2 = spawnshell(makeDeps);
  process2.wait();
  var process3 = spawnshell(makeExamples);
  process3.wait();
}


proc installDeps(lockFile: Toml) {
  var sourceList = genSourceList(lockFile);
  var source = [("https://github.com/spartee/mason-registry", "mason-registry-0.1.0")];
  getSrcCode(source);
}