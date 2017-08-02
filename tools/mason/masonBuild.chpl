
use TOML;
use Spawn;
use FileSystem;

proc main(args: [] string) {
  if isFile("Mason.lock") {
    makeTarget();
    makeTargetFiles();
  }
  else {
    writeln("Mason needs a lock file!");
  }
} 

proc makeTarget() {
  var makeTarget = "mkdir target";
  var result = spawnshell(makeTarget);
  result.wait();
}

// add if release turn optimizations on
proc makeTargetFiles() {
  var makeDebug  = "mkdir target/debug";
  var result = spawnshell(makeDebug);
  result.wait();

  var makeDeps = "mkdir target/debug/deps";
  var makeExamples = "mkdir target/debug/examples";
  var result2 = spawnshell(makeDeps);
  result2.wait();
  var result3 = spawnshell(makeExamples);
  result3.wait();
}