class C { var x : int; }

record R {
  var c : C;
  proc length return c.x;
  proc deinit() {
    writeln("destroying ", c);
    delete c;
  }
}

pragma "init copy fn"
proc chpl__initCopy(other : R) {
  pragma "no auto destroy"
  var ret : R;
  ret.c = new C(other.length);
  return ret;
}

config const earlyReturn = false;

iter foo1() {
  var state = new R(new C(0));
  var r1 = new R(new C(1));
  var r2 = new R(new C(2));

  yield r1;

  if earlyReturn {
    // destroy state, r2
    writeln("early return");
    return;
  }

  yield r2;

  // destroy state only
  writeln("return at end");
}

iter foo2() {
  var state = new R(new C(0));

  for i in 1..3 {
    yield new R(new C(i));
    if earlyReturn {
      writeln("early return");
      return;
    }
  }

  writeln("return at end");
}

iter foo3() {
  var state = new R(new C(0));

  for i in 1..3 {
    delete state.c;
    state.c = new C(i);
    yield state;
    if earlyReturn {
      writeln("early return");
      return;
    }
  }

  writeln("return at end");
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
}
