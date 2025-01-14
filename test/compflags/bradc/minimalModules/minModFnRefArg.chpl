use CTypes;

extern proc printf(f, x);
extern {
    const char* formatStr = "%d\n";
}

operator =(ref lhs, rhs) { __primitive("=", lhs, rhs); }

proc foo(ref x) {
  printf(formatStr, x);
  x = 2: int(8);
}

var x = 1: int(8);

foo(x);

printf(formatStr, x);
