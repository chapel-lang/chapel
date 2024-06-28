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

proc doSomething(c: C) { // Yes, generic.
  writeln("doing something else!");
  return 42 + c.x;
}

proc main() {
  {
    var captureArray: [1..10] int = foreach i in 1..10 do i*i;
    var theThunk = __primitive("create thunk", captureArray);
    writeln("Not yet, Ferb!");
    writeln(__primitive("force thunk", theThunk));
  }
  {
    var captureMe = 42;
    var theThunk = __primitive("create thunk", foreach i in 1..10 do i*captureMe);
    writeln("Not yet, Ferb!");
    writeln(__primitive("force thunk", theThunk));
  }
  {
    var captureArray: [1..10] int = foreach i in 1..10 do i*i;
    var theThunk = __primitive("create thunk", captureArray + 1);
    writeln("Not yet, Ferb!");
    writeln(__primitive("force thunk", theThunk));
  }
  {
    var theThunk = __primitive("create thunk", for i in 1..10 do for j in 1..10 do i*j);
    writeln("Not yet, Ferb!");
    writeln(__primitive("force thunk", theThunk));
  }
}
