use BlockDist;
use Memory;

config const numTasks = 4;
config const n = 100_000_000;
config const debug = false;

proc test1() {
  writeln("test1");
  const LocTaskSpace = {0..#numTasks};
  const DistTaskSpace = LocTaskSpace dmapped Block(LocTaskSpace);

  if debug {
    writeln("before allocation memoryUsed() == ", memoryUsed() / 1024.0 / 1024.0 / 1024.0, " GiB");
    startVerboseMem();
  }

  var allBucketKeys: [DistTaskSpace] [0..#n] uint(8);
  
  if debug {
    stopVerboseMem();
    writeln("after allocation memoryUsed() == ", memoryUsed() / 1024.0 / 1024.0 / 1024.0, " GiB");
  }

  writeln("test1 - OK");
}
test1();
