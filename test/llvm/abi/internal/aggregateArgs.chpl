//  * a by-value struct of at most 16 bytes with at most 4 scalar fields is
//    passed as scalars
//  * any other struct is passed as a pointer to a copy made by the caller
//  * a struct result over 16 bytes is returned through an sret pointer passed
//    as the first argument
//  * a smaller struct result is returned as its only scalar field, or by value
//  * Chapel records are passed by pointer, and
//    returned through a pointer passed as the last argument (REF) or, with
//    --no-return-by-ref, like any other struct (VAL)

use CTypes;

extern {
  typedef struct { int a; double b; } small2;
  typedef struct { int a, b, c, d; } four;
  typedef struct { int a, b, c, d, e; } big5;
  typedef struct { int a[2]; } arr2;
  typedef struct { small2 s; small2 t; } nested4;
  typedef struct { small2 s; small2 t; int c; } nested5;
  typedef struct { double x; } one;
  typedef struct { one o; } oneNested;
}

class C { var x: int; var next: unmanaged C?; }

record R { var a: int; var b: real; }
record Outer { var r: R; var c: int; }

// CHECK-LABEL: define internal double @takesComplex_chpl(double %{{[^,]+}}, double %{{[^)]+}})
proc takesComplex(z: complex) do return z.re + z.im;

// CHECK-LABEL: define internal { double, double } @returnsComplex_chpl(double %{{[^)]+}})
// CHECK: ret { double, double }
proc returnsComplex(x: real) do return x + 2.0i;

// CHECK-LABEL: define internal i64 @takesRange_chpl(i64 %{{[^,]+}}, i64 %{{[^)]+}})
proc takesRange(r: range) do return r.low + r.high;

// CHECK-LABEL: define internal i64 @takesClass_chpl(i32 %{{[^,]+}}, ptr %{{[^)]+}})
proc takesClass(c: unmanaged C) do return c.x;

// CHECK-LABEL: define internal %chpl____wide_C @returnsClass_chpl(i32 %{{[^,]+}}, ptr %{{[^)]+}})
// CHECK: ret %chpl____wide_C
proc returnsClass(c: unmanaged C) do return c.next!;

// CHECK-LABEL: define internal double @takesSmall_chpl(i32 %{{[^,]+}}, double %{{[^)]+}})
proc takesSmall(in s: small2) { s.a += 100; return s.a + s.b; }

// CHECK-LABEL: define internal %struct.small2 @returnsSmall_chpl(i64 %{{[^)]+}})
// CHECK: ret %struct.small2
proc returnsSmall(a: int) { var s: small2; s.a = a: c_int; s.b = 0.5; return s; }

// CHECK-LABEL: define internal i32 @takesFour_chpl(i32 %{{[^,]+}}, i32 %{{[^,]+}}, i32 %{{[^,]+}}, i32 %{{[^)]+}})
proc takesFour(in f: four) do return f.a + f.b + f.c + f.d;

// CHECK-LABEL: define internal i32 @takesBig_chpl(ptr %{{[^)]+}})
proc takesBig(in b: big5) { b.a += 100; return b.a + b.b + b.c + b.d + b.e; }

// CHECK-LABEL: define internal void @returnsBig_chpl(ptr {{[a-z_ ]*}}sret(%struct.big5) %{{[^,]+}}, i64 %{{[^)]+}})
// CHECK: ret void
proc returnsBig(a: int) { var b: big5; b.a = a: c_int; b.e = 5; return b; }

// CHECK-LABEL: define internal i32 @takesArr_chpl(ptr %{{[^)]+}})
proc takesArr(in x: arr2) do return x.a[0] + x.a[1];

// CHECK-LABEL: define internal double @takesNested4_chpl(ptr %{{[^)]+}})
proc takesNested4(in n: nested4) {
  n.s.a += 100;
  return n.s.a + n.s.b + n.t.a + n.t.b;
}

// CHECK-LABEL: define internal void @returnsNested4_chpl(ptr {{[a-z_ ]*}}sret(%struct.nested4) %{{[^,]+}}, i64 %{{[^)]+}})
// CHECK: ret void
proc returnsNested4(a: int) {
  var n: nested4; n.s.a = a: c_int; n.t.b = 0.25; return n;
}

// CHECK-LABEL: define internal i32 @takesNested5_chpl(ptr %{{[^)]+}})
proc takesNested5(in n: nested5) { n.s.a += 100; return n.s.a + n.t.a + n.c; }

// CHECK-LABEL: define internal double @takesRec_chpl(ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
proc takesRec(in r: R) { r.a += 100; return r.a + r.b; }

// CHECK-LABEL: define internal double @takesRecConst_chpl(ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
proc takesRecConst(r: R) do return r.a + r.b;

// REF-LABEL: define internal void @returnsRec_chpl(i64 %{{[^,]+}}, ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
// REF: ret void
// VAL-LABEL: define internal %R_chpl @returnsRec_chpl(i64 %{{[^)]+}})
// VAL: ret %R_chpl
proc returnsRec(a: int) do return new R(a, 0.5);

// CHECK-LABEL: define internal i64 @takesOuter_chpl(ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
proc takesOuter(in o: Outer) { o.r.a += 100; return o.r.a + o.c; }

// REF-LABEL: define internal void @returnsOuter_chpl(i64 %{{[^,]+}}, ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
// VAL-LABEL: define internal void @returnsOuter_chpl(ptr {{[a-z_ ]*}}sret(%Outer_chpl) %{{[^,]+}}, i64 %{{[^)]+}})
// CHECK: ret void
proc returnsOuter(a: int) do return new Outer(new R(a, 0.5), 1);

// CHECK-LABEL: define internal i64 @takesOwned_chpl(ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
proc takesOwned(o: owned C) do return o.x;

// REF-LABEL: define internal void @returnsOwned_chpl(i64 %{{[^,]+}}, ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
// REF: ret void
// VAL-LABEL: define internal ptr @returnsOwned_chpl(i64 %{{[^)]+}})
// VAL: ret ptr
proc returnsOwned(x: int) do return new C(x);

// CHECK-LABEL: define internal i64 @takesShared_chpl(ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
proc takesShared(s: shared C) do return s.x;

// REF-LABEL: define internal void @returnsShared_chpl(i64 %{{[^,]+}}, ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
// REF: ret void
// VAL-LABEL: define internal %_shared_C_chpl @returnsShared_chpl(i64 %{{[^)]+}})
// VAL: ret %_shared_C_chpl
proc returnsShared(x: int) do return new shared C(x);

// CHECK-LABEL: define internal double @returnsOne_chpl(double %{{[^)]+}})
proc returnsOne(x: real) { var o: one; o.x = x; return o; }

// CHECK-LABEL: define internal double @returnsOneNested_chpl(double %{{[^)]+}})
proc returnsOneNested(x: real) { var n: oneNested; n.o.x = x; return n; }

// CHECK-LABEL: define internal void @callAll_chpl()
proc callAll() {
  // CHECK: call {{.*}}{ double, double } @returnsComplex_chpl(double 1.000000e+00)
  // CHECK: call {{.*}}double @takesComplex_chpl(double %{{[^,]+}}, double %{{[^)]+}})
  writeln(takesComplex(returnsComplex(1.0)));

  // CHECK: call i64 @takesRange_chpl(i64 %{{[^,]+}}, i64 %{{[^)]+}})
  writeln(takesRange(1..4));

  var c = new unmanaged C(7, new unmanaged C(8));
  // CHECK: call %chpl____wide_C @returnsClass_chpl(i32 %{{[^,]+}}, ptr %{{[^)]+}})
  // CHECK: call i64 @takesClass_chpl(i32 %{{[^,]+}}, ptr %{{[^)]+}})
  writeln(takesClass(returnsClass(c)));
  on Locales[numLocales-1] do writeln(takesClass(returnsClass(c)));
  delete c.next, c;

  // CHECK: call %struct.small2 @returnsSmall_chpl(i64 3)
  var s = returnsSmall(3);
  // CHECK: call {{.*}}double @takesSmall_chpl(i32 %{{[^,]+}}, double %{{[^)]+}})
  writeln(takesSmall(s), " ", s.a);

  var f: four; f.a = 1; f.b = 2; f.c = 3; f.d = 4;
  // CHECK: call i32 @takesFour_chpl(i32 %{{[^,]+}}, i32 %{{[^,]+}}, i32 %{{[^,]+}}, i32 %{{[^)]+}})
  writeln(takesFour(f));

  // CHECK: call void @returnsBig_chpl(ptr {{[a-z_ ]*}}sret(%struct.big5) %{{[^,]+}}, i64 4)
  var b = returnsBig(4);
  // CHECK: call i32 @takesBig_chpl(ptr %{{[^)]+}})
  writeln(takesBig(b), " ", b.a);

  var x: arr2; x.a[0] = 20; x.a[1] = 22;
  // CHECK: call i32 @takesArr_chpl(ptr %{{[^)]+}})
  writeln(takesArr(x));

  // CHECK: call void @returnsNested4_chpl(ptr {{[a-z_ ]*}}sret(%struct.nested4) %{{[^,]+}}, i64 5)
  var n4 = returnsNested4(5);
  // CHECK: call {{.*}}double @takesNested4_chpl(ptr %{{[^)]+}})
  writeln(takesNested4(n4), " ", n4.s.a);

  var n5: nested5; n5.s.a = 6; n5.t.a = 7; n5.c = 8;
  // CHECK: call i32 @takesNested5_chpl(ptr %{{[^)]+}})
  writeln(takesNested5(n5), " ", n5.s.a);

  // REF: call void @returnsRec_chpl(i64 9, ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
  // VAL: call %R_chpl @returnsRec_chpl(i64 9)
  var r = returnsRec(9);
  // CHECK: call {{.*}}double @takesRec_chpl(ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
  // CHECK: call {{.*}}double @takesRecConst_chpl(ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
  writeln(takesRec(r), " ", takesRecConst(r), " ", r.a);

  // REF: call void @returnsOuter_chpl(i64 10, ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
  // VAL: call void @returnsOuter_chpl(ptr {{[a-z_ ]*}}sret(%Outer_chpl) %{{[^,]+}}, i64 10)
  var o = returnsOuter(10);
  // CHECK: call i64 @takesOuter_chpl(ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
  writeln(takesOuter(o), " ", o.r.a);

  // REF: call void @returnsOwned_chpl(i64 11, ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
  // VAL: call ptr @returnsOwned_chpl(i64 11)
  var ow = returnsOwned(11);
  // CHECK: call i64 @takesOwned_chpl(ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
  writeln(takesOwned(ow));

  // REF: call void @returnsShared_chpl(i64 12, ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
  // VAL: call %_shared_C_chpl @returnsShared_chpl(i64 12)
  var sh = returnsShared(12);
  // CHECK: call i64 @takesShared_chpl(ptr nonnull dereferenceable({{[0-9]+}}) %{{[^)]+}})
  writeln(takesShared(sh), " ", sh.x);

  // CHECK: call {{.*}}double @returnsOne_chpl(double 1.500000e+00)
  writeln(returnsOne(1.5).x);
  // CHECK: call {{.*}}double @returnsOneNested_chpl(double 2.500000e+00)
  writeln(returnsOneNested(2.5).o.x);
}
callAll();

// CHECK: {{^}}3.0{{$}}
// CHECK-NEXT: {{^}}5{{$}}
// CHECK-NEXT: {{^}}8{{$}}
// CHECK-NEXT: {{^}}8{{$}}
// CHECK-NEXT: {{^}}103.5 3{{$}}
// CHECK-NEXT: {{^}}10{{$}}
// CHECK-NEXT: {{^}}109 4{{$}}
// CHECK-NEXT: {{^}}42{{$}}
// CHECK-NEXT: {{^}}105.25 5{{$}}
// CHECK-NEXT: {{^}}121 6{{$}}
// CHECK-NEXT: {{^}}109.5 9.5 9{{$}}
// CHECK-NEXT: {{^}}111 10{{$}}
// CHECK-NEXT: {{^}}11{{$}}
// CHECK-NEXT: {{^}}12 12{{$}}
// CHECK-NEXT: {{^}}1.5{{$}}
// CHECK-NEXT: {{^}}2.5{{$}}
