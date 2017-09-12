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
// CHECK: store %A_chpl %{{[0-9]+}}, %A_chpl* %localConst_chpl
// CHECK-NEXT: %[[REG1:[0-9]+]] = bitcast %A_chpl* %localConst_chpl to i8*
// CHECK-NEXT: %{{[0-9]+}} = call {}* @llvm.invariant.start.p0i8(i64 8, i8* %[[REG1]])
    const localConst = new A(i*10);
    sum += localConst.a;
  }

  if n < 10 {
// CHECK: store %A_chpl %{{[0-9]+}}, %A_chpl* %localConst_chpl2
// CHECK-NEXT: %[[REG2:[0-9]+]] = bitcast %A_chpl* %localConst_chpl2 to i8*
// CHECK-NEXT: %{{[0-9]+}} = call {}* @llvm.invariant.start.p0i8(i64 8, i8* %[[REG2]])
    const localConst = new A(n*10);
    return localConst.a;
  }
  else {
// CHECK: store %A_chpl %{{[0-9]+}}, %A_chpl* %localConst_chpl3
// CHECK-NEXT: %[[REG3:[0-9]+]] = bitcast %A_chpl* %localConst_chpl3 to i8*
// CHECK-NEXT: %{{[0-9]+}} = call {}* @llvm.invariant.start.p0i8(i64 8, i8* %[[REG3]])
    const localConst = new A(n*5);
    return localConst.a;
  }
  return sum;
}

writeln(f(100));
