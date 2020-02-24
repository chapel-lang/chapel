use ExampleErrors;

config const n = 10;
config const t = 2;


iter myiter(nn: int, nt: int) {
  for i in 0..#nt {
    for j in i*nn..#nn {
      yield j;
    }
  }
}

iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
  coforall i in 0..#nt {
    for j in i*nn..#nn {
      yield j;
    }
  }
}

// coforall loop in leader should NOT get vector pragma
iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
  coforall i in 0..#nt {
    yield i*nn..#nn;
  }
}

// for loop in follower with yield should get vector pragma
iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
  for i in followThis {
    yield i;
  }
}

proc test() {
  try {
    writeln("before forall block");
    forall i in myiter(n,t) {
      if i == 1 then
        throw new owned StringError("test error");
    }
    writeln("after forall block");
  } catch errors: TaskErrors {
    for e in errors { 
      if e != nil {
        writeln("Caught group error e ", e!.message());
      }
    }
  } catch e {
    writeln("Caught other error ", e.message());
  }
}

test();
