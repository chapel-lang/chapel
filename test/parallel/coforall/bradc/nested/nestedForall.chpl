use DSIUtil;

config const numTasks=4;

iter leadfollow() {
  for i in 0..numLocales do
    yield i;
}

iter leadfollow(param tag: iterKind) where tag == iterKind.leader {
  coforall loc in Locales {
    on loc {
      coforall taskid in 0..#numTasks {
        const (lo,hi) = _computeBlock(1, numTasks, taskid, loc.id, loc.id, loc.id);
        yield {lo..hi};
      }
    }
  }
}

iter leadfollow(param tag: iterKind, followThis) where tag == iterKind.follower {
  for i in followThis {
    yield i;
  }
}


forall i in leadfollow() {
  writeln(here.id, " got ", i);
}
