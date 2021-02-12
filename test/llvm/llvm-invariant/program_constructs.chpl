// This test checks if llvm.invariant.start is added to
// Program constructs like if and while

record A
{
  var a : int;
}

proc f(n)
{
  var sum = 0;
  for i in 1..10
  {
// CHECK: call {{.*}} @init_chpl{{.*}}(%A_chpl* {{[^%]*}}[[NEW_TEMP:%[0-9a-zA-Z_]+]],
// CHECK: [[TMP1:%[0-9a-zA-Z_]+]] = load %A_chpl, %A_chpl* [[NEW_TEMP]]
// CHECK: store %A_chpl [[TMP1]], %A_chpl* [[TMP2:%[0-9a-zA-Z_]+]]
// CHECK: [[TMP3:%[0-9a-zA-Z_]+]] = load %A_chpl, %A_chpl* [[TMP2]]
// CHECK: store %A_chpl [[TMP3]], %A_chpl* [[PTR1:%[0-9a-zA-Z_]+]]
// CHECK: [[CAST1:%[0-9a-zA-Z_]+]] = bitcast %A_chpl* [[PTR1]] to i8*
// CHECK: call {}* @llvm.invariant.start.p0i8(i64 8, i8* [[CAST1]])
// CHECK: getelementptr
// CHECK-SAME:[[PTR1]]
    const localConst = new A(i*10);
    sum += localConst.a;
  }

  if n < 10 {
// CHECK: call {{.*}} @init_chpl{{.*}}(%A_chpl* {{[^%]*}}[[NEW_TEMP:%[0-9a-zA-Z_]+]],
// CHECK: [[TMP1:%[0-9a-zA-Z_]+]] = load %A_chpl, %A_chpl* [[NEW_TEMP]]
// CHECK: store %A_chpl [[TMP1]], %A_chpl* [[TMP2:%[0-9a-zA-Z_]+]]
// CHECK: [[TMP3:%[0-9a-zA-Z_]+]] = load %A_chpl, %A_chpl* [[TMP2]]
// CHECK: store %A_chpl [[TMP3]], %A_chpl* [[PTR2:%[0-9a-zA-Z_]+]]
// CHECK: [[CAST2:%[0-9a-zA-Z_]+]] = bitcast %A_chpl* [[PTR2]] to i8*
// CHECK: call {}* @llvm.invariant.start.p0i8(i64 8, i8* [[CAST2]])
// CHECK: getelementptr
// CHECK-SAME:[[PTR2]]
    const localConst = new A(n*10);
    return localConst.a;
  }
  else {
// CHECK: call {{.*}} @init_chpl{{.*}}(%A_chpl* {{[^%]*}}[[NEW_TEMP:%[0-9a-zA-Z_]+]],
// CHECK: [[TMP1:%[0-9a-zA-Z_]+]] = load %A_chpl, %A_chpl* [[NEW_TEMP]]
// CHECK: store %A_chpl [[TMP1]], %A_chpl* [[TMP2:%[0-9a-zA-Z_]+]]
// CHECK: [[TMP3:%[0-9a-zA-Z_]+]] = load %A_chpl, %A_chpl* [[TMP2]]
// CHECK: store %A_chpl [[TMP3]], %A_chpl* [[PTR3:%[0-9a-zA-Z_]+]]
// CHECK: [[CAST3:%[0-9a-zA-Z_]+]] = bitcast %A_chpl* [[PTR3]] to i8*
// CHECK: call {}* @llvm.invariant.start.p0i8(i64 8, i8* [[CAST3]])
// CHECK: getelementptr
// CHECK-SAME:[[PTR3]]
    const localConst = new A(n*5);
    return localConst.a;
  }
  return sum;
}

writeln(f(100));
