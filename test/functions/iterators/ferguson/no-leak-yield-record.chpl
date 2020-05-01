class C { var x : int; }

record R {
  var c : unmanaged C?;
  proc length return c!.x;
  proc deinit() {
    writeln("destroying ", c);
    delete c;
  }
}

pragma "init copy fn"
proc chpl__initCopy(other : R) {
  pragma "no auto destroy"
  var ret : R;
  writeln("copy/assign ", other.c);
  ret.c = new unmanaged C(other.length);
  return ret;
}

proc =(ref dst:R, src:R) {
  writeln("copy/assign ", src.c);
}


config const earlyReturn = false;

iter foo1() {
  var state = new R(new unmanaged C(0));
  ref rstate = state;
  var r1 = new R(new unmanaged C(1));
  var r2 = new R(new unmanaged C(2));

  yield r1;

  if earlyReturn {
    writeln("early return");
    // destroy state
    return;
  }

  yield r2;

  writeln("return at end");
  // destroy state
}

iter foo2() {
  var state = new R(new unmanaged C(0));
  ref rstate = state;

  for i in 1..3 {
    yield new R(new unmanaged C(i));
    if earlyReturn {
      writeln("early return");
      // destroy state
      return;
    }
  }

  writeln("return at end");
  // destroy state
}

proc makeR(i:int) {
  return new R(new unmanaged C(i));
}

iter foo3() {
  var state = new R(new unmanaged C(0));
  ref rstate = state;

  for i in 1..3 {
    yield makeR(i);
    if earlyReturn {
      writeln("early return");
      // destroy state
      return;
    }
  }

  writeln("return at end");
  // destroy state
}


iter foo4() {
  var state = new R(new unmanaged C(0));
  ref rstate = state;

  for i in 1..3 {
    delete state.c;
    state.c = new unmanaged C(i);
    yield state;
    if earlyReturn {
      writeln("early return");
      // destroy state
      return;
    }
  }

  writeln("return at end");
  // destroy state
}


proc main() {
  writeln("foo1");
  for i in foo1() {
    writeln("in loop, got ", i);
  }

  writeln("foo2");
  for j in foo2() {
    writeln("in loop, got ", j);
  }

  writeln("foo3");
  for j in foo3() {
    writeln("in loop, got ", j);
  }

  writeln("foo4");
  for j in foo4() {
    writeln("in loop, got ", j);
  }

}
