// This test checks if llvm.invariant.start is added to function local constants
// Normally primitive type constants could be used, but chapel compiler
// performs some constant propagation, which doesn't happen in case of records.

record A
{
  var a : int;
}

// CHECK: @f_chpl
// CHECK: call void @init_chpl{{.*}}(%A_chpl* {{[^%]*}}[[NEW_TEMP:%.*]],
// CHECK: [[TMP1:%.*]] = load %A_chpl, %A_chpl* [[NEW_TEMP]]
// CHECK: store %A_chpl [[TMP1]], %A_chpl* [[TMP2:%.*]]
// CHECK: [[TMP3:%.*]] = load %A_chpl, %A_chpl* [[TMP2]]
// CHECK: store %A_chpl [[TMP3]], %A_chpl* [[TMP4:%.*]]
// CHECK: [[CAST:%.*]] = bitcast %A_chpl* [[TMP4]] to i8*
// CHECK: call {}* @llvm.invariant.start.p0i8(i64 8, i8* [[CAST]])
// CHECK: getelementptr
// CHECK-SAME:[[TMP4]]
// CHECK: load
// CHECK: ret
proc f(n)
{
  const localConst = new A(n*10);
  return localConst.a;
}

writeln(f(100));
