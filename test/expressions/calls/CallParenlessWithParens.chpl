proc bar { return 0; }

proc test() {
  var x = bar();
  writeln(x);
}
test();

