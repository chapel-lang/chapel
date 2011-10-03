proc foo(type T, a:T):int where T == int checked {
  return a+a;
}
