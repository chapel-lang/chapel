record R { var x: int; }
class C { var x: int; }

proc doSomething(input: int) {
  writeln("doing something!");
  return 42 + input;
}

proc doSomething(r: R) {
  writeln("doing something else!");
  return 42 + r.x;
}

proc doSomething(c: C) { // Yes, generic -- unspecified ownership for c.
  writeln("doing something else!");
  return 42 + c.x;
}

// Create a function to make sure that the variable is not a global.
proc main() {
  {
    var captureMe = 1;
    var theThunk = __primitive("create thunk", doSomething(captureMe));
    writeln("Not yet, Ferb!");
    writeln(__primitive("force thunk", theThunk));
  }
  {
    var captureMe = new R(1);
    var theThunk = __primitive("create thunk", doSomething(captureMe));
    writeln("Not yet, Ferb!");
    writeln(__primitive("force thunk", theThunk));
  }
  {
    var captureMe = new owned C(1);
    var theThunk = __primitive("create thunk", doSomething(captureMe));
    writeln("Not yet, Ferb!");
    writeln(__primitive("force thunk", theThunk));
  }
  {
    var captureMe = new shared C(1);
    var theThunk = __primitive("create thunk", doSomething(captureMe));
    writeln("Not yet, Ferb!");
    writeln(__primitive("force thunk", theThunk));
  }
}
