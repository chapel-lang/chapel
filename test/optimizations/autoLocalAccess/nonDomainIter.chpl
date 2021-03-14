use common;

class C {
  var i = 10;
    iter these() {
      yield i;
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone {
      yield i;
    }
}

var A = createArr({1..10}, int);
var B = createArr({1..10}, int);

var c = new C();

forall i in c {
  A[i] = 2 * B[i];
}
