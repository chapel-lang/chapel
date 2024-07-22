var A: [1..10] int;

record rec {
  proc this() ref : real {
    return A[1];
  }
}

var r = new rec();
writeln(r());
