use ExampleErrors;

config const n = 10;
config const t = 2;


iter myiter(nn: int, nt: int) throws {

  throw new owned StringError("Test error");

  for i in 0..#nt {
    for j in i*nn..#nn {
      yield j;
    }
  }
}

iter myiter(nn: int, nt: int, param tag: iterKind) throws where tag == iterKind.standalone {
  throw new owned StringError("Test error");

  coforall i in 0..#nt {
    for j in i*nn..#nn {
      yield j;
    }
  }
}

// coforall loop in leader should NOT get vector pragma
iter myiter(nn: int, nt: int, param tag: iterKind) throws where tag == iterKind.leader {
  throw new owned StringError("Test error");

  coforall i in 0..#nt {
    yield i*nn..#nn;
  }
}

// for loop in follower with yield should get vector pragma
iter myiter(nn:int, nt: int, followThis, param tag: iterKind) throws where tag == iterKind.follower {
  for i in followThis {
    yield i;
  }
}

proc test() {
  try {
    writeln("before forall block");
    forall i in myiter(n,t) {
      writeln(i);
    }
    writeln("after forall block");
  } catch e {
    writeln("Caught error ", e.message());
  }
}

test();
