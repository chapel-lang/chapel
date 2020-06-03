// We need a class because a class pointer (as opposed to a class
// object) is a scalar.
class MyClass {
  var ifield: int;
  var rfield: real;
};

var ivar: int;
var rvar: real;

proc f() {
  var cvar: unmanaged MyClass = new unmanaged MyClass();

  ivar = 42;
  // Look for an access of the int var, and remember its TBAA access tag.
  // CHECK: store i64 42
  // CHECK-SAME: !tbaa ![[INTACC:[0-9]+]]

  rvar = 3.14;
  // Look for an access of the real var, and remember its TBAA access tag.
  // CHECK: store double 3.14
  // CHECK-SAME: !tbaa ![[REALACC:[0-9]+]]

  delete cvar;
  cvar = new unmanaged MyClass();
  return cvar;
  // Look for an access of the class pointer, and remember its TBAA access tag.
  // CHECK: store %chpl_MyClass_chpl{{[0-9]*}}_object*
  // CHECK-SAME: !tbaa ![[CLSPTRACC:[0-9]+]]
}

var got = f();
writeln(f(), ", ", ivar, ", ", rvar);
delete got;

// Look for the tree of TBAA type descriptors that we will need to
// validate the TBAA access tags we remember from above.
//
// TBAA root node:
// CHECK-DAG: ![[CHPLTYPES:[0-9]+]] = !{!"Chapel types"}
//
// Scalar TBAA type descriptors:  reference to parent type only
// CHECK-DAG: ![[UNIONS:[0-9]+]] = !{!"all unions", ![[CHPLTYPES]], i64 0}
// CHECK-DAG: ![[INT:[0-9]+]] = !{!"int64_t", ![[UNIONS]], i64 0}
// CHECK-DAG: ![[REAL:[0-9]+]] = !{!"_real64", ![[UNIONS]], i64 0}
// CHECK-DAG: ![[CVOIDPTR:[0-9]+]] = !{!"C void ptr", ![[UNIONS]], i64 0}
//
// Note that class pointers are scalars.
// CHECK-DAG: ![[OBJECT:[0-9]+]] = !{!"object_chpl{{[0-9]*}}", ![[CVOIDPTR]], i64 0}
// CHECK-DAG: ![[CLSPTR:[0-9]+]] = !{!"MyClass_chpl{{[0-9]*}}", ![[OBJECT]], i64 0}
//
// Now validate those access tags.
// Scalar accesses are always at offset zero.
// CHECK-DAG: ![[INTACC]] = !{![[INT]], ![[INT]], i64 0}
// CHECK-DAG: ![[REALACC]] = !{![[REAL]], ![[REAL]], i64 0}
// CHECK-DAG: ![[CLSPTRACC]] = !{![[CLSPTR]], ![[CLSPTR]], i64 0}
