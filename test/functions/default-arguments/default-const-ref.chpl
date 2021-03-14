var global:int = 1;

proc foo(const ref a:int, const ref b:int = a) {
  global = 10;
  writeln(b);
}

foo(global);
