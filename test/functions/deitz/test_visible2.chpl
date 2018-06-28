class C {
  var x : int;
}

var c = new borrowed C();

proc foo() {
  proc bar(c : borrowed C) {
    writeln("in bar");
  }
  writeln("in foo");
}

foo();
bar(c);
