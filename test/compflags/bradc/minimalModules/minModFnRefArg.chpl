inline proc |(param a: bool, param b: bool) param return __primitive("|", a, b);

proc isInt(type t) param return
  (t == int(8)) | (t == int(16)) | (t == int(32)) | (t == int(64));

inline proc _defaultOf(type t) param where isInt(t) return 0:t;
inline proc _defaultOf(type t) param where t: string return defaultStringValue;

pragma "trivial assignment"
inline proc =(ref a: int(?w), b: int(w)) { __primitive("=", a, b); }

extern proc printf(f:c_string, x...);
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
  inline proc _build_tuple(x...)
    return x;
  
