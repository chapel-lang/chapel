// This test checks if llvm.invariant.start is added to function local constants
// Normally primitive type constants could be used, but chapel compiler
// performs some constant propagation, which doesn't happen in case of records.

record A
{
  var a : int;
}

// CHECK: @f_chpl
// CHECK: call void @init_chpl{{.*}}(%A_chpl* [[PTR:%.*]],
// CHECK: [[CAST:%.*]] = bitcast %A_chpl* [[PTR]] to i8*
// CHECK: call {}* @llvm.invariant.start.p0i8(i64 8, i8* [[CAST]])
// CHECK: getelementptr
// CHECK-SAME:[[PTR]]
// CHECK: load
// CHECK: ret
proc f(n)
{
  const localConst = new A(n*10);
  return localConst.a;
}

writeln(f(100));
