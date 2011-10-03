proc foo(type T, a:int):T where T == int checked {
  return a+a;
}
