record BBB {
  var a,b,c: int;
  proc initialize() {
    writeln("BBB.initialize()");
  }
}
record AAA {
  var b: BBB;
  proc initialize() {
    writeln("AAA.initialize()");
  }
}

proc main() {
  var a: AAA;
  writeln(a);
}


