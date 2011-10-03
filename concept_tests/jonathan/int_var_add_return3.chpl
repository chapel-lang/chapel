proc foo(type T, a:T):T where T == int checked {
  return a+a;
}
