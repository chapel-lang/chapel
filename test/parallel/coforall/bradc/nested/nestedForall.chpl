use BlockDist;

config const numTasks=4;

def leadfollow() {
  for i in 0..numLocales do
    yield i;
}

def leadfollow(param tag: iterator) where tag == iterator.leader {
  coforall loc in Locales {
    on loc {
      coforall taskid in 0..#numTasks {
        const (lo,hi) = chpl_computeBlock(loc.id, 1, loc.id, loc.id, numTasks, taskid);
        yield [lo..hi];
      }
    }
  }
}

def leadfollow(param tag: iterator, follower) where tag == iterator.follower {
  for i in follower {
    yield i;
  }
}


forall i in leadfollow() {
  writeln(here.id, " got ", i);
}
