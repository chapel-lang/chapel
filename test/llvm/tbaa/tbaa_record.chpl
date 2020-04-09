record MyRecord {
  var ival: int;
  var rval: real;
};

proc f(i: int, r: real) {
  var myRecord: MyRecord;

  // First make sure we are looking inside the record.
  // CHECK: getelementptr

  myRecord.ival = i;
  // Look for dereference of int field, and remember its TBAA access tag.
  // CHECK: store i64
  // CHECK-SAME: !tbaa ![[INTVIAREC:[0-9]+]]

  myRecord.rval = r;
  // Look for dereference of real field, and remember its TBAA access tag.
  // CHECK: store double
  // CHECK-SAME: !tbaa ![[REALVIAREC:[0-9]+]]

  return myRecord;
}

writeln(f(42, 3.14));

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
//
// Record TBAA type descriptor:  reference to each field's type
// CHECK-DAG: ![[REC:[0-9]+]] = !{!"MyRecord_chpl", ![[INT]], i64 0, ![[REAL]], i64 {{[0-9]+}}}
//
// Now validate those access tags.
// The int is at offset zero because it is the first field.
// CHECK-DAG: ![[INTVIAREC]] = !{![[REC]], ![[INT]], i64 0}
// CHECK-DAG: ![[REALVIAREC]] = !{![[REC]], ![[REAL]], i64 {{[0-9]+}}}
