config const numTasks=4;

iter leadfollow() {
  for i in 0..numLocales do
    yield i;
}

iter leadfollow(param tag: iterator) where tag == iterator.leader {
  coforall loc in Locales {
    on loc {
      coforall taskid in 0..#numTasks {
        const lo = loc.id*numTasks + taskid;
        yield lo;
      }
    }
  }
}

iter leadfollow(param tag: iterator, follower) where tag == iterator.follower {
  yield follower;
}


forall i in leadfollow() {
  writeln(here.id, " got ", i);
}
