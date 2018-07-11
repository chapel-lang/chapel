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
// CHECK: call {{.*}} @_construct_A_chpl
// CHECK: store
// CHECK: load
// CHECK-DAG: call {}* @llvm.invariant.start.p0i8(i64 8, i8* [[CAST1:%.*]])
// CHECK-DAG: [[CAST1]] = bitcast %A_chpl* [[PTR1:%.*]] to i8*
// CHECK_DAG: store %A_chpl {{%.*}}, %A_chpl* [[PTR1]]
// CHECK: getelementptr
// CHECK-SAME:[[PTR1]]
    const localConst = new A(i*10);
    sum += localConst.a;
  }

  if n < 10 {
// CHECK: call {{.*}} @_construct_A_chpl
// CHECK: store
// CHECK: load
// CHECK-DAG: call {}* @llvm.invariant.start.p0i8(i64 8, i8* [[CAST2:%.*]])
// CHECK-DAG: [[CAST2]] = bitcast %A_chpl* [[PTR2:%.*]] to i8*
// CHECK_DAG: store %A_chpl {{%.*}}, %A_chpl* [[PTR2]]
// CHECK: getelementptr
// CHECK-SAME:[[PTR2]]
    const localConst = new A(n*10);
    return localConst.a;
  }
  else {
// CHECK: call {{.*}} @_construct_A_chpl
// CHECK: store
// CHECK: load
// CHECK-DAG: call {}* @llvm.invariant.start.p0i8(i64 8, i8* [[CAST3:%.*]])
// CHECK-DAG: [[CAST3]] = bitcast %A_chpl* [[PTR3:%.*]] to i8*
// CHECK_DAG: store %A_chpl {{%.*}}, %A_chpl* [[PTR3]]
// CHECK: getelementptr
// CHECK-SAME:[[PTR3]]
    const localConst = new A(n*5);
    return localConst.a;
  }
  return sum;
}

writeln(f(100));
