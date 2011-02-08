use DSIUtil;

config const numTasks=4;

iter leadfollow() {
  for i in 0..numLocales do
    yield i;
}

iter leadfollow(param tag: iterator) where tag == iterator.leader {
  coforall loc in Locales {
    on loc {
      coforall taskid in 0..#numTasks {
        const (lo,hi) = _computeBlock(1, numTasks, taskid, loc.id, loc.id, loc.id);
        yield [lo..hi];
      }
    }
  }
}

iter leadfollow(param tag: iterator, follower) where tag == iterator.follower {
  for i in follower {
    yield i;
  }
}


forall i in leadfollow() {
  writeln(here.id, " got ", i);
}
