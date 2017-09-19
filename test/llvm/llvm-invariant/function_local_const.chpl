// This test checks if llvm.invariant.start is added to function local constants
// Normally primitive type constants could be used, but chapel compiler
// performs some constant propagation, which doesn't happen in case of records.

record A
{
  var a : int;
}


// CHECK: store %A_chpl %{{[0-9]+}}, %A_chpl* %localConst_chpl
// CHECK-NEXT: %[[REG1:[0-9]+]] = bitcast %A_chpl* %localConst_chpl to i8*
// CHECK-NEXT: %{{[0-9]+}} = call {}* @llvm.invariant.start.p0i8(i64 8, i8* %[[REG1]])

proc f(n)
{
  const localConst = new A(n*10);
  return localConst.a;
}

writeln(f(100));
