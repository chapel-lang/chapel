class MyClass {
  var ival: int;
  var rval: real;
};

var myClass: MyClass;

proc f(i: int, r: real) {
  // First make sure we are looking inside the class object.
  // CHECK: getelementptr

  myClass.ival = i;
  // Look for dereference of int field, and remember its TBAA access tag.
  // CHECK: store i64
  // CHECK-SAME: !tbaa ![[INTVIACLS:[0-9]+]]

  myClass.rval = r;
  // Look for dereference of real field, and remember its TBAA access tag.
  // CHECK: store double
  // CHECK-SAME: !tbaa ![[REALVIACLS:[0-9]+]]
}

myClass = new MyClass;
f(42, 3.14);
writeln(myClass);

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
// Note that "object" is a scalar type descriptor because it is a pointer.
// CHECK-DAG: ![[OBJECT:[0-9]+]] = !{!"object", ![[CVOIDPTR]], i64 0}
//
// Class object TBAA type descriptor:  reference to superclass and each field
// CHECK-DAG: ![[CLS:[0-9]+]] = !{!"chpl_MyClass_chpl_object", ![[OBJECT]], i64 0, ![[INT]], i64 {{[0-9]+}}, ![[REAL]], i64 {{[0-9]+}}}
//
// Now validate those access tags.
// The int is not at offset zero because the object superclass comes first.
// CHECK-DAG: ![[INTVIACLS]] = !{![[CLS]], ![[INT]], i64 {{[0-9]+}}}
// CHECK-DAG: ![[REALVIACLS]] = !{![[CLS]], ![[REAL]], i64 {{[0-9]+}}}
