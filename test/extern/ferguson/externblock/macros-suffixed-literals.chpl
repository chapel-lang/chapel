extern {
  #define MY_INT_CONSTANT 11
  #define MY_UINT_CONSTANT 12u

  #define MY_LONG_CONSTANT 13l
  #define MY_ULONG_CONSTANT 14ul
  #define MY_ULONG_CONSTANT2 15lu

  #define MY_LONG_LONG_CONSTANT 16ll
  #define MY_ULONG_LONG_CONSTANT 17ull
  #define MY_ULONG_LONG_CONSTANT2 18llu

  #define MY_FLOAT_CONSTANT 19.0f

  #define MY_DOUBLE_CONSTANT 20.0

  #define MY_LONG_DOUBLE_CONSTANT 21.0l
}

proc f(type t, x) {
  writeln(x);
  assert(t:string == x.type:string);
}

f(c_int, MY_INT_CONSTANT);
f(c_uint, MY_UINT_CONSTANT);
writeln();

f(c_long, MY_LONG_CONSTANT);
f(c_ulong, MY_ULONG_CONSTANT);
f(c_ulong, MY_ULONG_CONSTANT2);
writeln();

f(c_longlong, MY_LONG_LONG_CONSTANT);
f(c_ulonglong, MY_ULONG_LONG_CONSTANT);
f(c_ulonglong, MY_ULONG_LONG_CONSTANT2);
writeln();

f(c_float, MY_FLOAT_CONSTANT);
writeln();

f(c_double, MY_DOUBLE_CONSTANT);
writeln();

f(c_double, MY_LONG_DOUBLE_CONSTANT);
writeln();
