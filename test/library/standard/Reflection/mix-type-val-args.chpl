proc foo(type t, x) {
  writeln(t: string);
  writeln(x);
}

writeln(Reflection.canResolve("foo", int, 3));
