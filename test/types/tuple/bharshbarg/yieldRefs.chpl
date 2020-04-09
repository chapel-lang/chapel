record R {
  var x : int;
}

var g : R;

proc test(args: R ...) {
  for a in args {
    writeln(a);
    g.x += 1;
    writeln(a);
    assert(a == g);
    writeln("----");
  }
}

test(g,g,g);
