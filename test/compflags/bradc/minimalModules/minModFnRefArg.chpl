
extern proc printf(f, x);
operator =(ref lhs, rhs) { __primitive("=", lhs, rhs); }

proc foo(ref x) {
  printf("%d\n", x);
  x = 2: int(8);
}

var x = 1: int(8);

foo(x);

printf("%d\n", x);
