use CommDiagnostics, CompressedSparseLayout;

config param compressRows = true;

config const countComms = true;

const D = {1..9, 1..9};
var SD: sparse subdomain(D) dmapped if compressRows then new csrLayout()
                                                    else new cscLayout()
                            = [i in 1..9] (i,i);
var A: [SD] real;

for i in 1..9 do
  A[i,i] = i + i/10.0;

on Locales[numLocales-1] {
  startCommDiagnostics();
  const B = A;
  stopCommDiagnostics();
  if countComms then printCommDiagnosticsTable();
  local {
    for (i,j) in D {
      const bij = B[i,j];
      const error = (((i == j) && (bij != (i + j/10.0))) ||
                     (i != j) && (bij != 0.0));
      if error {
        halt("B[",(i,j), "] = ", bij, " vs. ", i + j/10.0);
      }
    }
  }
  writeMatrix(B);
}

const SD2 = SD;
var A2: [SD2] real;

for i in 1..9 do
  A2[i,i] = i + i/10.0;

on Locales[numLocales-1] {
  resetCommDiagnostics();
  startCommDiagnostics();
  var B = A2;
  stopCommDiagnostics();
  if countComms then printCommDiagnosticsTable();

  local {
    for ij in D {
      if B.domain.contains(ij) then
        B[ij] = 2;
    }
  }
  writeMatrix(B);
}

proc writeMatrix(X) {
  for i in 1..9 {
    for j in 1..9 {
      write(X[i,j], " ");
    }
    writeln();
  }
}
