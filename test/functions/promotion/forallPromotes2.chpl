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
    for i in followThis(0) do
      yield i+x;
  }
}

proc foo(x: int) {
  writeln(x);
}

proc foo(x: real) {
  writeln(x);
}

proc foo(xy:(int, real)) {
  writeln(xy(0), " ", xy(1));
}

var myC = new borrowed C();

/*
forall (i,j) in zip(myC, 4..6) do
  writeln(i, " ", j);

forall (i,j) in zip(4..6, myC) do
  writeln(i, " ", j);
*/

writeln("Trying to promote foo:");
foo([(i,j) in zip(myC, 4..6)] (i, 1.0*j));
foo([(i,j) in zip(4..6, myC)] (i, 1.0*j));


//foo([(i,j) in zip(1..3, 4..6)] (i, 1.0*j));
//foo([(i,j) in zip(4..6, 1..3)] (i, 1.0*j));
