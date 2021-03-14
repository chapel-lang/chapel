proc foo(x, y: int) where y {
  compilerError("shouldn't get here");
}

foo(1, 2);
