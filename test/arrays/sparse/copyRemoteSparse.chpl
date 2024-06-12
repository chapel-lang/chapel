const D = {1..9, 1..9};

var SD: sparse subdomain(D) = [i in 1..9] (i,i);

var A: [SD] real;

for i in 1..9 do
  A[i,i] = i + i/10.0;

on Locales[numLocales-1] {
  const B = A;
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
  var B = A2;
  local {
    for i in 1..9 {
      for j in 1..9 {
        if B.domain.contains(i,j) then
          B[i,j] = 2;
      }
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
