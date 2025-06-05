use CompressedSparseLayout, CommDiagnostics;

config const n = 9;


{
  var SD: sparse subdomain({1..n, 1..n}) = [i in 1..n] (i,i);
  var A: [SD] real;

  on Locales[1] {
    startCommDiagnostics();
    const B = A;
    stopCommDiagnostics();
  }

  printCommDiagnosticsTable();
}

resetCommDiagnostics();

{
  var SD: sparse subdomain({1..n, 1..n}) dmapped new csrLayout()
        = [i in 1..n] (i,i);
  var A: [SD] real;

  on Locales[1] {
    startCommDiagnostics();
    const B = A;
    stopCommDiagnostics();
  }

  printCommDiagnosticsTable();
}
