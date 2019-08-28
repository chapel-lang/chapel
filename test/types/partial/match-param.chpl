record R {
  type T;
  param p : T;
}

proc foo(x : R(?T, "hello")) {
  writeln("hello-version");
  writeln(x.type:string);
  writeln();
}

proc foo(x : R) {
  writeln("generic");
  writeln(x.type:string);
  writeln();
}

foo(new R(int, 100));
foo(new R(string, "hello"));

