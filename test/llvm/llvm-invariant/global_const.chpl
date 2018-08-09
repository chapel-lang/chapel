// This test checks whether we add llvm.invariant.start to global const variables

// CHECK: call void @init_chpl{{.*}}(%A_chpl* [[NEW_TEMP:%.*]],
// CHECK: [[TMP1:%.*]] = load %A_chpl, %A_chpl* [[NEW_TEMP]]
// CHECK: store %A_chpl [[TMP1]], %A_chpl* [[TMP2:%.*]]
// CHECK: [[TMP3:%.*]] = load %A_chpl, %A_chpl* [[TMP2]]
// CHECK: store %A_chpl [[TMP3]], %A_chpl* @globalConst_chpl
// CHECK: %{{[0-9]+}} = call {}* @llvm.invariant.start.p0i8(i64 8, i8* bitcast (%A_chpl* @globalConst_chpl to i8*))

record A
{
  var x : int;
}

const globalConst = new A(1000);
writeln(globalConst);
