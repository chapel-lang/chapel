use common;

class C {
  var i = 10;
    iter these() {
      yield i;
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone {
      yield i;
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      yield (1..10, );
    }

    iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
      for i in followThis[0] do yield i;
    }
}

var A = createArr({1..10}, int);
var B = createArr({1..10}, int);

var c = new C();

forall (i, loopIdx) in zip(c, 1..) {
  A[i] = 2 * B[i] * loopIdx;
}
