module ambiguousNestedModule {
  module M {
    config param p = 1;
    config const c = 2;
    config var   v = 3;
    config type  t = int;
  }
  config param p = 4;
  config const c = 5;
  config var   v = 6;
  config type  t = real;

  writeln((M.p, M.c, M.v, M.t:string, p, c, v, t:string));
}
