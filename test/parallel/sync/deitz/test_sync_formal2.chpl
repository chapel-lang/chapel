var i: int = 2;

proc foo(s: sync int) {
  s.writeEF(i);
  i += 1;
}

var s: sync int;
foo(s);
writeln(s.readFE());
foo(s);
writeln(s.readFF());
