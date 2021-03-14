// Test deadlock detection

var done: sync bool;

proc foo(u, v) {
  var s: sync int = u;

  begin {
    writeln("2: initial value is ", s.readFE());
    done.writeEF(true);
    writeln("2: value is now ", s.readFE());
    done.writeEF(true);
  }
  if done.readFE() then writeln("1: writing ", v);
  if done.readFE() then s.writeEF(v);
}

foo(1, 5);
