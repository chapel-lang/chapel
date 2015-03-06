// Test deadlock detection

var done: sync bool;

proc foo(u, v) {
  var s: sync int = u;

  begin {
    writeln("2: initial value is ", s.readFE());
    done = true;
    writeln("2: value is now ", s.readFE());
    done = true;
  }
  if done then writeln("1: writing ", v);
  if done then s = v;
}

foo(1, 5);
