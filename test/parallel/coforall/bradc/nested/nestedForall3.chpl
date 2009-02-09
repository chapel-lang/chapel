use BlockDist;

config const numTasks=4;

coforall loc in Locales {
  on loc {
    coforall taskid in 0..#numTasks {
      const (lo,hi) = _computeBlock(loc.id, 1, loc.id, loc.id, numTasks, taskid);
      writeln("yielding ", [lo..hi]);
    }
  }
}

