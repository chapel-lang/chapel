// This test checks whether we add llvm.invariant.start to global const variables

// CHECK: store %A_chpl {{.*}}, %A_chpl* @globalConst_chpl
// CHECK: %{{[0-9]+}} = call {}* @llvm.invariant.start.p0i8(i64 8, i8* bitcast (%A_chpl* @globalConst_chpl to i8*))

record A
{
  var x : int;
}

const globalConst = new A(1000);
writeln(globalConst);
