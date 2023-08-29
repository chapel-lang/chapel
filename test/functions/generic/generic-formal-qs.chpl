record R { type t; }
proc a(arg: R) { } // warning asking this to be written as 'arg: R(?)'
proc b(arg: R(?)) { }
a(new R(int));
b(new R(int));

proc getGenericType() type {
  return R(?);
}
proc c(arg: getGenericType()) { }
proc d(arg: getGenericType()(?)) { }
c(new R(int));
d(new R(int));
