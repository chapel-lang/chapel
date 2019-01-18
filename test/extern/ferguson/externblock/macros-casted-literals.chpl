extern {
  typedef long my_long_t;
  #define MY_TYPEDEF_CONSTANT ((my_long_t) 10)

  #define MY_LONG_CONSTANT ((long) 10)
  #define MY_ULONG_CONSTANT ((unsigned long) 10)
  #define MY_SLONG_CONSTANT ((signed long) 10)

  #define MY_UNSIGNED_CONSTANT ((unsigned) 10)
  #define MY_SIGNED_CONSTANT ((signed) 10)
  #define MY_INT_CONSTANT ((int) 10)
  #define MY_UINT_CONSTANT ((unsigned int) 10)
  #define MY_SINT_CONSTANT ((signed int) 10)

  #define MY_SHORT_CONSTANT ((short) 10)
  #define MY_USHORT_CONSTANT ((unsigned short) 10)
  #define MY_SSHORT_CONSTANT ((signed short) 10)

  #define MY_CHAR_CONSTANT ((char) 10)
  #define MY_SCHAR_CONSTANT ((signed char) 10)
  #define MY_UCHAR_CONSTANT ((unsigned char) 10)
}

proc f(type t, x) {
  assert(t:string == x.type:string);
  writeln(x);
}

f(c_long, MY_TYPEDEF_CONSTANT);
writeln();

f(c_long, MY_LONG_CONSTANT);
f(c_ulong, MY_ULONG_CONSTANT);
f(c_long, MY_SLONG_CONSTANT);
writeln();

f(c_uint, MY_UNSIGNED_CONSTANT);
f(c_int, MY_SIGNED_CONSTANT);
f(c_int, MY_INT_CONSTANT);
f(c_uint, MY_UINT_CONSTANT);
f(c_int, MY_SINT_CONSTANT);
writeln();

f(c_short, MY_SHORT_CONSTANT);
f(c_ushort, MY_USHORT_CONSTANT);
f(c_short, MY_SSHORT_CONSTANT);
writeln();

f(c_char, MY_CHAR_CONSTANT);
f(c_schar, MY_SCHAR_CONSTANT);
f(c_uchar, MY_UCHAR_CONSTANT);
writeln();
