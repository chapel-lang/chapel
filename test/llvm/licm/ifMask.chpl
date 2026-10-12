
proc ifMask(ref Res, A, B) {
  // CHECK-LABEL: @ifMask_chpl
  // CHECK-SAME: %Res,
  // CHECK-SAME: %A,
  // CHECK-SAME: %B)
  // CHECK: entry:

  // CHECK: %[[INSTANCE_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %A
  // CHECK-NEXT: %[[INSTANCE_A:.+]] = load ptr, ptr %[[INSTANCE_PTR_A]]
  // CHECK: %[[DATA_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_A]]
  // CHECK-NEXT: %[[DATA_A:.+]] = load ptr, ptr %[[DATA_PTR_A]]

  // CHECK: %[[INSTANCE_PTR_B:.+]] = getelementptr inbounds {{.+}} ptr %B
  // CHECK-NEXT: %[[INSTANCE_B:.+]] = load ptr, ptr %[[INSTANCE_PTR_B]]
  // CHECK: %[[DATA_PTR_B:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_B]]
  // CHECK-NEXT: %[[DATA_B:.+]] = load ptr, ptr %[[DATA_PTR_B]]

  foreach i in A.domain {
    // CHECK: [[LOOP:.+]]:
    // CHECK: %[[IDX:.+]] = phi i64

    // CHECK: %[[ELEM_PTR_A:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_A]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_A:.+]] = load {{.+}}, ptr %[[ELEM_PTR_A]]
    // CHECK: %[[RESULT:.+]] = fcmp {{.+}} %[[ELEM_A]]
    // CHECK: br i1 %[[RESULT]], label %[[THEN:.+]], label %[[AFTER:.+]]

    // CHECK: [[THEN]]:
    // CHECK-NEXT: %[[ELEM_PTR_B:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_B]]
    // CHECK-SAME: i64 %[[IDX]]
    // CHECK-NEXT: %[[ELEM_B:.+]] = load {{.+}}, ptr %[[ELEM_PTR_B]]
    // CHECK-NEXT: br label %[[AFTER]]

    // CHECK: [[AFTER]]:
    // CHECK-NEXT: %[[PHI_RESULT:.+]] = phi
    // CHECK: %[[INSTANCE_PTR_Res:.+]] = getelementptr inbounds {{.+}} ptr %Res
    // CHECK-NEXT: %[[INSTANCE_Res:.+]] = load ptr, ptr %[[INSTANCE_PTR_Res]]
    // CHECK: %[[DATA_PTR_Res:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_Res]]
    // CHECK-NEXT: %[[DATA_Res:.+]] = load ptr, ptr %[[DATA_PTR_Res]]
    // CHECK: %[[ELEM_PTR_Res:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_Res]]
    // CHECK-SAME: i64 %[[IDX]]
    // CHECK: store {{.+}} %[[PHI_RESULT]], ptr %[[ELEM_PTR_Res]]


    Res[i] = if A[i] >= 0:A.eltType then B[i] else 0:A.eltType;
    // CHECK: br i1 {{.+}} label %[[LOOP]]
  }
}

config const N = 10;
proc main() {
  var A: [1..N] real = 1..N;
  var B: [1..N] real = 1..N;
  var Res: [1..N] real;

  ifMask(Res, A, B);
  writeln("ifMask: ", Res);
}
