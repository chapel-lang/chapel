proc t2(param x) param { return x + 12; }
proc main() {
  var x = 12;
  writeln(t2#(x));
}
