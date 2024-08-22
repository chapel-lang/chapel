record R {
  var x: int;

  proc deinit() {
    writeln("deinitializing R with value ", x);
  }
}

class C {
  var x: int;

  proc deinit() {
    writeln("deinitializing C with value ", x);
  }
}

{
  var myThunk = __primitive("create thunk", new R(42));
  writeln(__primitive("force thunk", myThunk));
}

{
  var capturedR = new R(43);
  var myThunk = __primitive("create thunk", capturedR);
  writeln(__primitive("force thunk", myThunk));
}

{
  var myThunk = __primitive("create thunk", new shared C(42));
  writeln(__primitive("force thunk", myThunk));
}

{
  var caputredC = new shared C(43);
  var myThunk = __primitive("create thunk", caputredC);
  writeln(__primitive("force thunk", myThunk));
}
