// This test verifies that no-alias sets allow LLVM optimization
// It is a basic test that the appropriate LLVM metadata is emitted
// and that LLVM passes are able to use the metadata.

config const n = 10;

proc main() {
  var A: [1..10] int;
  var B: [1..10] int;

  test(n, A, B);
}

// CHECK: void @test
proc test(n:int, ref a, ref b) {
  // CHECK-SAME: {

  // Check that the loop was eliminated
  // CHECK-NOT: phi

  // Verify that the first load is decorated with alias.scope and noalias
  // CHECK: load
  // CHECK-SAME: !alias.scope
  // CHECK-SAME: !noalias 

  for i in 1..n {
    a[1] += b[1];
  }
  // Check that the loop was eliminated
  // CHECK: mul
  // CHECK: store
  // CHECK: }
}
