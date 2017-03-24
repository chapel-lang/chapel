class C {
  var x = 1;
  var y = 2;
  var z = 3;

  iter these() {
    writeln("Calling serial iterator");
    yield x;
    yield y;
    yield z;
  }

  /*
  iter these(param tag: iterKind) where tag == iterKind.standalone {
    writeln("Calling parallel iterator (which is actually serial)");
    yield x;
    yield y;
    yield z;
  }
  */

  iter these(param tag: iterKind) where tag == iterKind.leader {
    writeln("Calling leader iterator (which is actually serial)");
    yield (0..z-x,);
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    for i in followThis(1) do
      yield i+x;
  }
}

var myC = new C();

for c in myC do
  writeln(c);

forall c in myC do
  writeln(c);

proc foo(x: int) {
  writeln(x);
}

writeln("Trying to promote foo():");
foo(myC);
