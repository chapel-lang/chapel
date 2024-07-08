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


class C { type t; }
record WR { type t; }

proc e(arg: owned C) { }
e(new C(int));

proc f(arg: WR(R)) { }
f(new WR(R(int)));


proc g(arg: domain) { }
g({1..10});

proc h(arg: domain(?)) { }
h({1..10});
