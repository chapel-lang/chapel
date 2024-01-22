proc foo(const ref x: int(32)) {
  writeln(x, " ", x.type:string);
}

foo(42);
