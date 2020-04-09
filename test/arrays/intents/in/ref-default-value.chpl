
record R {
  var x : int;
  proc poke() {
    this.x += 100;
  }
}

var Global, Master : [1..4] R;
ref RefGlobal = Global;

proc helper() ref {
  return Global;
}

proc testRefGlobal(in A = RefGlobal) {
  A[1].poke();
  writeln(Global);
}
proc testHelper(in A = helper()) {
  A[1].poke();
  writeln(Global);
}

writeln("----- testRefGlobal() -----");
testRefGlobal();
writeln();

writeln("----- testRefGlobal(RefGlobal) -----");
testRefGlobal(RefGlobal);
writeln();

writeln("----- testHelper() -----");
testHelper();
writeln();

writeln("----- testHelper(helper()) -----");
testHelper(helper());
writeln();

writeln("----- Final Global -----");
writeln(Global);
