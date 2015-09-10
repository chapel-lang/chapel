inline proc |(param a: bool, param b: bool) param return __primitive("|", a, b);

proc isInt(type t) param return
  (t == int(8)) | (t == int(16)) | (t == int(32)) | (t == int(64));

inline proc _defaultOf(type t) param where isInt(t) return 0:t;
inline proc _defaultOf(type t) param where t: string return defaultStringValue;

var xyz = 1: int(8);
extern proc printf(f:c_string, x...);
printf("%d\n", xyz);

  pragma "tuple" record _tuple {
    param size : int;
  }
  
  pragma "build tuple"
  inline proc _build_tuple(x...)
    return x;
  
