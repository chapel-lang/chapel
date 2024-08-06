class C {
  var x: int;

  proc init(x: int) {
    this.x = x;
    init this;
    writeln("C.init");
  }
}

proc executeIfTrue(cond: bool, in thunk: _thunkRecord) {
  var temp: thunkToReturnType(thunk.type);
  writeln("In execute if true, with value: ", cond);
  if cond {
    temp = __primitive("force thunk", thunk);
  }
  return temp;
}

writeln(executeIfTrue(true, __primitive("create thunk", new C?(42))));
writeln(executeIfTrue(false, __primitive("create thunk", new C?(42))));
