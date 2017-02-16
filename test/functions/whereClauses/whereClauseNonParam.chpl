proc foo(x: int) where x {
  compilerError("shouldn't get here");
}

foo(1);
