
extern proc printf(f:c_string, x);
operator =(ref lhs, rhs) { __primitive(c"=", lhs, rhs); }

proc foo(ref x) {
  printf(c"%d\n", x);
  x = 2: int(8);
}

var x = 1: int(8);

foo(x);

printf(c"%d\n", x);
