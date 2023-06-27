class C {
  var x : int;
}
var cObj = new C();
var c = cObj.borrow();

proc foo() {
  proc bar(c : borrowed C) {
    writeln("in bar");
  }
  writeln("in foo");
}

bar(c);
