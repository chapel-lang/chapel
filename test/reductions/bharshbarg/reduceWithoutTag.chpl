
iter bad(n : int) {
  for i in 1..n do yield i;
}

iter bad(param tag: iterKind, n : int) where tag == iterKind.leader {
  var dom = {1..n};
  forall i in dom do yield i;
}

iter bad(param tag: iterKind, n : int, followThis) where tag == iterKind.follower {
  yield followThis;
}

// Expecting 55, instead seeing 0
writeln(+ reduce bad(10));

iter good(n : int) {
  for i in 1..n do yield i;
}

iter good(param tag: iterKind, n : int) where tag == iterKind.leader {
  var dom = {1..n};
  for follow in dom.these(iterKind.leader) {
    for i in dom.these(iterKind.follower, follow) {
      yield i;
    }
  }
}

iter good(param tag: iterKind, n : int, followThis) where tag == iterKind.follower {
  yield followThis;
}

writeln(+ reduce good(10));
