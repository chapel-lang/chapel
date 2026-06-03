// A should get LICMed
proc sum1(A) {
  // CHECK-LABEL: sum1_chpl
  // CHECK-SAME: %A)
  // CHECK: entry:

  // CHECK: %[[INSTANCE_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %A
  // CHECK-NEXT: %[[INSTANCE_A:.+]] = load ptr, ptr %[[INSTANCE_PTR_A]]
  // CHECK: %[[DATA_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_A]]
  // CHECK-NEXT: %[[DATA_A:.+]] = load ptr, ptr %[[DATA_PTR_A]]

  var s = 0:A.eltType;
  foreach i in A.domain with (ref s) {
    // CHECK: [[LOOP:.+]]:
    // CHECK: %[[IDX:.+]] = phi i64

    // CHECK: %[[ELEM_PTR_A:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_A]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_A:.+]] = load {{.+}}, ptr %[[ELEM_PTR_A]]
    // CHECK: fadd {{.+}} %[[ELEM_A]]
    s += A[i];
    // CHECK: br i1 {{.+}} label %[[LOOP]]
  }
  return s;
}

// A should get LICMed
proc sum2(const A) {
  // CHECK-LABEL: sum2_chpl
  // CHECK-SAME: %A)
  // CHECK: entry:

  // CHECK: %[[INSTANCE_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %A
  // CHECK-NEXT: %[[INSTANCE_A:.+]] = load ptr, ptr %[[INSTANCE_PTR_A]]
  // CHECK: %[[DATA_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_A]]
  // CHECK-NEXT: %[[DATA_A:.+]] = load ptr, ptr %[[DATA_PTR_A]]

  var s = 0:A.eltType;
  foreach i in A.domain with (ref s) {
    // CHECK: [[LOOP:.+]]:
    // CHECK: %[[IDX:.+]] = phi i64

    // CHECK: %[[ELEM_PTR_A:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_A]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_A:.+]] = load {{.+}}, ptr %[[ELEM_PTR_A]]
    // CHECK: fadd {{.+}} %[[ELEM_A]]
    s += A[i];
    // CHECK: br i1 {{.+}} label %[[LOOP]]
  }
  return s;
}


// A should NOT get LICMed
proc sum3(const ref A) {
  // CHECK-LABEL: sum3_chpl
  // CHECK-SAME: %A)
  // CHECK: entry:

  var s = 0:A.eltType;
  foreach i in A.domain with (ref s) {
    // CHECK: [[LOOP:.+]]:
    // CHECK: %[[IDX:.+]] = phi i64

    // CHECK: %[[INSTANCE_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %A
    // CHECK-NEXT: %[[INSTANCE_A:.+]] = load ptr, ptr %[[INSTANCE_PTR_A]]
    // CHECK: %[[DATA_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_A]]
    // CHECK-NEXT: %[[DATA_A:.+]] = load ptr, ptr %[[DATA_PTR_A]]

    // CHECK: %[[ELEM_PTR_A:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_A]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_A:.+]] = load {{.+}}, ptr %[[ELEM_PTR_A]]
    // CHECK: fadd {{.+}} %[[ELEM_A]]
    s += A[i];
    // CHECK: br i1 {{.+}} label %[[LOOP]]
  }
  return s;
}

// A should NOT get LICMed
proc sum4(ref A) {
  // CHECK-LABEL: sum4_chpl
  // CHECK-SAME: %A)
  // CHECK: entry:

  var s = 0:A.eltType;
  foreach i in A.domain with (ref s) {
    // CHECK: [[LOOP:.+]]:
    // CHECK: %[[IDX:.+]] = phi i64

    // CHECK: %[[INSTANCE_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %A
    // CHECK-NEXT: %[[INSTANCE_A:.+]] = load ptr, ptr %[[INSTANCE_PTR_A]]
    // CHECK: %[[DATA_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_A]]
    // CHECK-NEXT: %[[DATA_A:.+]] = load ptr, ptr %[[DATA_PTR_A]]

    // CHECK: %[[ELEM_PTR_A:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_A]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_A:.+]] = load {{.+}}, ptr %[[ELEM_PTR_A]]
    // CHECK: fadd {{.+}} %[[ELEM_A]]
    s += A[i];
    // CHECK: br i1 {{.+}} label %[[LOOP]]
  }
  return s;
}


config const N = 10;
proc main() {
  var A: [1..N] real = 1..N;
  var Res: [1..N] real;

  writeln("sum1: ", sum1(A));
  writeln("sum2: ", sum2(A));
  writeln("sum3: ", sum3(A));
  writeln("sum4: ", sum4(A));
}
