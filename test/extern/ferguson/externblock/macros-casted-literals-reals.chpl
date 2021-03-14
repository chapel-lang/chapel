extern {
  #define MY_FLOAT_CONSTANT ((float) 10.0)

  #define MY_DOUBLE_CONSTANT ((double) 10.0)
}

proc f(type t, x) {
  assert(t:string == x.type:string);
  writeln(x);
}

f(c_float, MY_FLOAT_CONSTANT);
writeln();

f(c_double, MY_DOUBLE_CONSTANT);
writeln();
