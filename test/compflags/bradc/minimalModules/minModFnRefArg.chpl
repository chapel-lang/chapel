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

  pragma "tuple" record _tuple {
    param size : int;
  }
  
  pragma "build tuple"
  inline proc _build_tuple_always(x ...?size)
    return x;
  
