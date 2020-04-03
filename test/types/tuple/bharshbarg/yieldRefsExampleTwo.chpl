record R {
  var x : int;
}

proc test(in args : 3*R) {
  var i = 0;
  for a in args {
    writeln("before: ", a);
    writeln("mutating...");
    args(i).x *= 10;
    writeln("after:  ", a);
    writeln();
    i += 1;
  }
  writeln(args);
}

proc main() {
  var A = new R(1);
  var B = new R(2);
  var C = new R(3);

  var tup = (A, B, C);
  test(tup);
}
