var global: int;

record rec {
  var A: int;

  proc this() ref : real {
    return global;
  }
}

var r = new rec();
writeln(r());
