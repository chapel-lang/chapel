
proc throwError(i) throws {
  if i == 1 then
    throw new Error("abc");
}

iter myiter() {
  yield 5;
}

iter myiter(param tag) where tag == iterKind.standalone {
  coforall idx in 1..2 {
    yield idx;
    throwError(idx);
  }
}

proc test() {
  try {
    writeln("before forall block");
    forall i in myiter() {
      // this may execute once or twice
    }
    writeln("after forall block");
  } catch e {
    writeln("test caught ", e);
  }
}

test();
