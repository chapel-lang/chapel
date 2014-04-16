pragma "trivial assignment"
inline proc =(ref a: int(?w), b: int(w)) { __primitive("=", a, b); }

extern proc printf(x...);
proc foo(ref x) {
  printf("%d\n", x);
  x = 2: int(8);
}

var x = 1: int(8);

foo(x);

printf("%d\n", x);
