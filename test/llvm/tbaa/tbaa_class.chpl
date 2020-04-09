class MyClass {
  var ival: int;
  var rval: real;
};

var class1 = new MyClass();
var class2 = new MyClass();

var myClass: borrowed MyClass = class1.borrow();

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

myClass = class2.borrow();
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
// CHECK-DAG: ![[INT32:[0-9]+]] = !{!"int32_t", ![[UNIONS]], i64 0}
// CHECK-DAG: ![[INT:[0-9]+]] = !{!"int64_t", ![[UNIONS]], i64 0}
// CHECK-DAG: ![[REAL:[0-9]+]] = !{!"_real64", ![[UNIONS]], i64 0}
//
// Class object TBAA type descriptors:  reference to superclass and each field
// CHECK-DAG: ![[OBJOBJ:[0-9]+]] = !{!"chpl_object{{[0-9]*}}_chpl_object", ![[INT32]], i64 0}
// CHECK-DAG: ![[CLSOBJ:[0-9]+]] = !{!"chpl_MyClass_chpl{{[0-9]*}}_object", ![[OBJOBJ]], i64 0, ![[INT]], i64 {{[0-9]+}}, ![[REAL]], i64 {{[0-9]+}}}
//
// Now validate those access tags.
// The int is not at offset zero because the object superclass comes first.
// CHECK-DAG: ![[INTVIACLS]] = !{![[CLSOBJ]], ![[INT]], i64 {{[0-9]+}}}
// CHECK-DAG: ![[REALVIACLS]] = !{![[CLSOBJ]], ![[REAL]], i64 {{[0-9]+}}}
