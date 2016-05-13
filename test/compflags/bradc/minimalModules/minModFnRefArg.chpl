inline proc _defaultOf(type t) param where t == int(8) return 0:t;

extern proc printf(f:c_string, x...);
proc foo(ref x) {
  printf("%d\n", x);
  x = 2: int(8);
}

var x = 1: int(8);

foo(x);

printf("%d\n", x);
