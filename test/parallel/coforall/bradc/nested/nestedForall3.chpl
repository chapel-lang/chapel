use DSIUtil;

config const numTasks=4;

coforall loc in Locales {
  on loc {
    coforall taskid in 0..#numTasks {
      const (lo,hi) = _computeBlock(1, numTasks, taskid, loc.id, loc.id, loc.id);
      writeln("yielding ", {lo..hi});
    }
  }
}

