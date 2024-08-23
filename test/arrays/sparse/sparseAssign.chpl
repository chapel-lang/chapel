use LayoutCS;

const D = {1..9, 1..9};

var DiagCOO: sparse subdomain(D) = [i in 1..9] (i,i);
var DiagCOO2: sparse subdomain(D) = DiagCOO;

var DiagCSR: sparse subdomain(D) dmapped new cs() = [i in 1..9] (i,i);
var DiagCSR2: sparse subdomain(D) dmapped new cs() = DiagCSR;

exercise(DiagCSR, DiagCSR2);
exercise(DiagCOO, DiagCOO2);

proc exercise(ref SD1, ref SD2) {
  var A: [SD1] real;

  for i in 1..9 do
    A[i,i] = i + i/10.0;

  writeMatrix(A);

  on Locales.last {
    var A2: [SD2] real;
    A2 = A;

    writeMatrix(A2);

    A2 += 1.0;

    writeMatrix(A2);

    A = A2;
  }

  writeMatrix(A);
}

proc writeMatrix(X) {
  for i in 1..9 {
    for j in 1..9 {
      write(X[i,j], " ");
    }
    writeln();
  }
  writeln();
}
