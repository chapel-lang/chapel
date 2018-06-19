
var A:[1..4] int;


class Impl {
  iter these() ref {
    for i in A.these() {
      yield i;
    }
  }
  iter these(param tag: iterKind) where tag == iterKind.standalone {
    for i in A.these(tag) {
      yield i;
    }
  }
  iter these(param tag: iterKind) where tag == iterKind.leader {
    for followThis in A.these(tag) {
      yield followThis;
    }
  }
  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    for i in A.these(tag, followThis) {
      yield i;
    }
  }
}

record R {
  forwarding var impl:unmanaged Impl;
}

proc test() {
  var r = new R(new unmanaged Impl());
  forall i in r {
    writeln(i);
  }
  delete r.impl;
}

test();
