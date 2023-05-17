// This test checks whether we add llvm.invariant.start to global const variables

// CHECK: store %A_chpl {{.*}}, ptr @globalConst_chpl
// CHECK: %{{[0-9]+}} = call ptr @llvm.invariant.start.p0(i64 8, ptr @globalConst_chpl)

record A
{
  var x : int;
}

const globalConst = new A(1000);
writeln(globalConst);
