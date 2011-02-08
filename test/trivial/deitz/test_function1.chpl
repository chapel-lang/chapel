record R {
  var x;
  var y;
}

proc foo(a, b) {
  var r = new R(a, b);
  writeln(r);
}

foo(2, "hi");
foo(3.0, "bye");
