use BlockDist;

config const numTasks=4;

def iterate() {
  coforall loc in Locales {
    on loc {
      coforall taskid in 0..#numTasks {
        const (lo,hi) = chpl_computeBlock(loc.id, 1, loc.id, loc.id, numTasks, taskid);
        yield [lo..hi];
      }
    }
  }
}

for i in iterate() {
  writeln(here.id, " got ", i);
}
