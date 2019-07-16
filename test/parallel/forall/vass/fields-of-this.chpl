
const RNG = 1..4;

record R {
  var data: [RNG] int;

  proc method() {
    forall i in RNG {
      data[i] = i;     // the field 'data' is implicitly passed by ref
    }
  }
}

proc main() {
  var r = new R();
  r.method();
  writeln(r);
}
