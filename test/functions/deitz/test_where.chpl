proc foo(type t, x: int(32)) where x.type == t {
  writeln("foo positive", x);
}

proc foo(type t, x: int(32)) where x.type != t {
  writeln("foo negative", x);
}

foo(int(32), 12);
foo(real, 12);
