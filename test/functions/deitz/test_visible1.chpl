class C {
  var x : int;
}

var c = new C();

proc foo() {
  proc bar(c : C) {
    writeln("in bar");
  }
  writeln("in foo");
}

bar(c);
