class C {
  var x : int;
}

var c = new borrowed C();

proc foo() {
  proc bar(c : C) {
    writeln("in bar");
  }
  writeln("in foo");
}

foo();
bar(c);
