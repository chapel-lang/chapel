extern {
  #define MY_INT_CONSTANT (11 << 1)
  #define MY_INT_CONSTANT2 (((int)12) << 1)
  #define MY_UINT_CONSTANT (13u << 1)
  #define MY_UINT_CONSTANT2 (((unsigned)14) << 1)

  #define MY_LONG_CONSTANT (15l << 1)
  #define MY_LONG_CONSTANT2 (((long)16) << 1)
  #define MY_ULONG_CONSTANT (17ul << 1)
  #define MY_ULONG_CONSTANT2 (18lu << 1)
  #define MY_ULONG_CONSTANT3 (((unsigned long)19) << 1)

  #define MY_LONG_LONG_CONSTANT (20ll << 1)
  #define MY_LONG_LONG_CONSTANT2 (((long long)21) << 1)
  #define MY_ULONG_LONG_CONSTANT (22ull << 1)
  #define MY_ULONG_LONG_CONSTANT2 (23llu << 1)
  #define MY_ULONG_LONG_CONSTANT3 (((unsigned long long)24) << 1)
}

proc f(type t, x) {
  writeln(x);
  assert(t:string == x.type:string);
}

f(c_int, MY_INT_CONSTANT);
f(c_int, MY_INT_CONSTANT2);
f(c_uint, MY_UINT_CONSTANT);
f(c_uint, MY_UINT_CONSTANT2);
writeln();

f(c_long, MY_LONG_CONSTANT);
f(c_long, MY_LONG_CONSTANT2);
f(c_ulong, MY_ULONG_CONSTANT);
f(c_ulong, MY_ULONG_CONSTANT2);
f(c_ulong, MY_ULONG_CONSTANT3);
writeln();

f(c_longlong, MY_LONG_LONG_CONSTANT);
f(c_longlong, MY_LONG_LONG_CONSTANT2);
f(c_ulonglong, MY_ULONG_LONG_CONSTANT);
f(c_ulonglong, MY_ULONG_LONG_CONSTANT2);
f(c_ulonglong, MY_ULONG_LONG_CONSTANT3);
writeln();
