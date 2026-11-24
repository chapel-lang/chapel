// This test checks if llvm.invariant.start is added to function local constants
// Normally primitive type constants could be used, but chapel compiler
// performs some constant propagation, which doesn't happen in case of records.

record A
{
  var a : int;
}


proc calculate(const ref a: A) {
  return a.a;
}

// CHECK: @f_chpl
// CHECK: call void @init_chpl{{.*}}(%function_local_const_A_chpl* {{[^%]*}}[[NEW_TEMP:%[0-9a-zA-Z_]+]],
// CHECK: [[TMP1:%[0-9a-zA-Z_]+]] = load %function_local_const_A_chpl, %function_local_const_A_chpl* [[NEW_TEMP]]
// CHECK: store %function_local_const_A_chpl [[TMP1]], %function_local_const_A_chpl* [[TMP2:%[0-9a-zA-Z_]+]]
// CHECK: [[TMP3:%[0-9a-zA-Z_]+]] = load %function_local_const_A_chpl, %function_local_const_A_chpl* [[TMP2]]
// CHECK: store %function_local_const_A_chpl [[TMP3]], %function_local_const_A_chpl* [[TMP4:%[0-9a-zA-Z_]+]]
// CHECK: [[CAST:%[0-9a-zA-Z_]+]] = bitcast %function_local_const_A_chpl* [[TMP4]] to i8*
// CHECK: call {}* @llvm.invariant.start.p0i8(i64 8, i8* [[CAST]])
// CHECK: @calculate_chpl
// CHECK-SAME:[[TMP4]]
proc f(n)
{
  const localConst = new A(n*10);
  return calculate(localConst);
}

writeln(f(100));
