
// A should get LICMed, Res should NOT get LICMed
// A is passed by default intent (const), so its metadata can be LICMed
// Res is passed by ref, so its metadata cannot be LICMed
proc mulScalar1(ref Res, A, scalar) {
  // CHECK-LABEL: mulScalar1_chpl
  // CHECK-SAME: %Res,
  // CHECK-SAME: %A,
  // CHECK: entry:

  // CHECK: %[[INSTANCE_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %A
  // CHECK-NEXT: %[[INSTANCE_A:.+]] = load ptr, ptr %[[INSTANCE_PTR_A]]
  // CHECK: %[[DATA_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_A]]
  // CHECK-NEXT: %[[DATA_A:.+]] = load ptr, ptr %[[DATA_PTR_A]]

  foreach i in A.domain {
    // CHECK: [[LOOP:.+]]:
    // CHECK: %[[IDX:.+]] = phi i64

    // CHECK: %[[INSTANCE_PTR_Res:.+]] = getelementptr inbounds {{.+}} ptr %Res
    // CHECK-NEXT: %[[INSTANCE_Res:.+]] = load ptr, ptr %[[INSTANCE_PTR_Res]]
    // CHECK: %[[DATA_PTR_Res:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_Res]]
    // CHECK-NEXT: %[[DATA_Res:.+]] = load ptr, ptr %[[DATA_PTR_Res]]

    // CHECK: %[[ELEM_PTR_Res:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_Res]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_PTR_A:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_A]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_A:.+]] = load {{.+}}, ptr %[[ELEM_PTR_A]]
    // CHECK: %[[RESULT:.+]] = fmul {{.+}} %[[ELEM_A]]
    // CHECK-NEXT: store {{.+}} %[[RESULT]], ptr %[[ELEM_PTR_Res]]
    Res[i] = A[i] * scalar;

    // CHECK: br i1 {{.+}} label %[[LOOP]]
  }
}

// A should get LICMed, Res should NOT get LICMed
// A is passed by const, so its metadata can be LICMed
// Res is passed by ref, so its metadata cannot be LICMed
proc mulScalar2(ref Res, const A, scalar) {
  // CHECK-LABEL: mulScalar2_chpl
  // CHECK-SAME: %Res,
  // CHECK-SAME: %A,
  // CHECK: entry:

  // CHECK: %[[INSTANCE_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %A
  // CHECK-NEXT: %[[INSTANCE_A:.+]] = load ptr, ptr %[[INSTANCE_PTR_A]]
  // CHECK: %[[DATA_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_A]]
  // CHECK-NEXT: %[[DATA_A:.+]] = load ptr, ptr %[[DATA_PTR_A]]

  foreach i in A.domain {
    // CHECK: [[LOOP:.+]]:
    // CHECK: %[[IDX:.+]] = phi i64

    // CHECK: %[[INSTANCE_PTR_Res:.+]] = getelementptr inbounds {{.+}} ptr %Res
    // CHECK-NEXT: %[[INSTANCE_Res:.+]] = load ptr, ptr %[[INSTANCE_PTR_Res]]
    // CHECK: %[[DATA_PTR_Res:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_Res]]
    // CHECK-NEXT: %[[DATA_Res:.+]] = load ptr, ptr %[[DATA_PTR_Res]]

    // CHECK: %[[ELEM_PTR_Res:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_Res]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_PTR_A:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_A]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_A:.+]] = load {{.+}}, ptr %[[ELEM_PTR_A]]
    // CHECK: %[[RESULT:.+]] = fmul {{.+}} %[[ELEM_A]]
    // CHECK-NEXT: store {{.+}} %[[RESULT]], ptr %[[ELEM_PTR_Res]]
    Res[i] = A[i] * scalar;

    // CHECK: br i1 {{.+}} label %[[LOOP]]
  }
}

// A should NOT get LICMed, Res should NOT get LICMed
// A is passed by const ref, so its metadata cannot be LICMed
// Res is passed by ref, so its metadata cannot be LICMed
proc mulScalar3(ref Res, const ref A, scalar) {
  // CHECK-LABEL: mulScalar3_chpl
  // CHECK-SAME: %Res,
  // CHECK-SAME: %A,
  // CHECK: entry:

  foreach i in A.domain {
    // CHECK: [[LOOP:.+]]:
    // CHECK: %[[IDX:.+]] = phi i64

    // CHECK: %[[INSTANCE_PTR_Res:.+]] = getelementptr inbounds {{.+}} ptr %Res
    // CHECK-NEXT: %[[INSTANCE_Res:.+]] = load ptr, ptr %[[INSTANCE_PTR_Res]]
    // CHECK: %[[DATA_PTR_Res:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_Res]]
    // CHECK-NEXT: %[[DATA_Res:.+]] = load ptr, ptr %[[DATA_PTR_Res]]

    // CHECK: %[[ELEM_PTR_Res:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_Res]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[INSTANCE_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %A
    // CHECK-NEXT: %[[INSTANCE_A:.+]] = load ptr, ptr %[[INSTANCE_PTR_A]]
    // CHECK: %[[DATA_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_A]]
    // CHECK-NEXT: %[[DATA_A:.+]] = load ptr, ptr %[[DATA_PTR_A]]

    // CHECK: %[[ELEM_PTR_A:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_A]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_A:.+]] = load {{.+}}, ptr %[[ELEM_PTR_A]]
    // CHECK: %[[RESULT:.+]] = fmul {{.+}} %[[ELEM_A]]
    // CHECK-NEXT: store {{.+}} %[[RESULT]], ptr %[[ELEM_PTR_Res]]
    Res[i] = A[i] * scalar;

    // CHECK: br i1 {{.+}} label %[[LOOP]]
  }
}
// A should NOT get LICMed, Res should NOT get LICMed
// A is passed by ref, so its metadata cannot be LICMed
// Res is passed by ref, so its metadata cannot be LICMed
proc mulScalar4(ref Res, ref A, scalar) {
  // CHECK-LABEL: mulScalar4_chpl
  // CHECK-SAME: %Res,
  // CHECK-SAME: %A,
  // CHECK: entry:

  foreach i in A.domain {
    // CHECK: [[LOOP:.+]]:
    // CHECK: %[[IDX:.+]] = phi i64

    // CHECK: %[[INSTANCE_PTR_Res:.+]] = getelementptr inbounds {{.+}} ptr %Res
    // CHECK-NEXT: %[[INSTANCE_Res:.+]] = load ptr, ptr %[[INSTANCE_PTR_Res]]
    // CHECK: %[[DATA_PTR_Res:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_Res]]
    // CHECK-NEXT: %[[DATA_Res:.+]] = load ptr, ptr %[[DATA_PTR_Res]]

    // CHECK: %[[ELEM_PTR_Res:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_Res]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[INSTANCE_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %A
    // CHECK-NEXT: %[[INSTANCE_A:.+]] = load ptr, ptr %[[INSTANCE_PTR_A]]
    // CHECK: %[[DATA_PTR_A:.+]] = getelementptr inbounds {{.+}} ptr %[[INSTANCE_A]]
    // CHECK-NEXT: %[[DATA_A:.+]] = load ptr, ptr %[[DATA_PTR_A]]

    // CHECK: %[[ELEM_PTR_A:.+]] = getelementptr inbounds
    // CHECK-SAME: ptr %[[DATA_A]]
    // CHECK-SAME: i64 %[[IDX]]

    // CHECK: %[[ELEM_A:.+]] = load {{.+}}, ptr %[[ELEM_PTR_A]]
    // CHECK: %[[RESULT:.+]] = fmul {{.+}} %[[ELEM_A]]
    // CHECK-NEXT: store {{.+}} %[[RESULT]], ptr %[[ELEM_PTR_Res]]
    Res[i] = A[i] * scalar;

    // CHECK: br i1 {{.+}} label %[[LOOP]]
  }
}

config const N = 10;
proc main() {
  var A: [1..N] real = 1..N;
  var Res: [1..N] real;


  mulScalar1(Res, A, 2.0);
  writeln("mulScalar1: ", Res);
  mulScalar2(Res, A, 2.0);
  writeln("mulScalar2: ", Res);
  mulScalar3(Res, A, 2.0);
  writeln("mulScalar3: ", Res);
  mulScalar4(Res, A, 2.0);
  writeln("mulScalar4: ", Res);
}
