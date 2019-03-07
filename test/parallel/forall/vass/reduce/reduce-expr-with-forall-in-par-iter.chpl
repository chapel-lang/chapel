// beefed-up version of test/reductions/bharshbarg/reduceWithoutTag.chpl

use StencilDist;

var Dom = {1..10, 1..10};
var Space = Dom dmapped Stencil(Dom, fluff=(1,1,), periodic=true);
var AAA : [Space] int;
[a in AAA] a = 10;


iter myiter(n : int) {
  for i in 1..n do yield i;
}

iter myiter(param tag: iterKind, n : int) where tag == iterKind.leader {
  forall i in AAA do yield i;
}

iter myiter(param tag: iterKind, n : int, followThis) where tag == iterKind.follower {
  yield followThis;
}

// Expecting 55, instead seeing 0
writeln(+ reduce myiter(10));

var result = 7;
forall kkk in myiter(10) with (+ reduce result) {
  result += kkk;
}
writeln(result);
