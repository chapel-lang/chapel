use LayoutCS;

config param compressRows = true, checkError = false;

var D = {1..9, 1..9};
var SD: sparse subdomain(D) dmapped new cs(compressRows=compressRows);

for i in 1..9 do
  for j in -1..1 do
    if D.contains(i, i+j) then
      SD += (i, i+j);

var A: [SD] real;

for (i,j) in SD do
  A[i,j] = i + j/10.0;

for i in SD.rows() do
  writeln(i);
writeln();

for j in SD.cols() do
  writeln(j);
writeln();

for i in A.rows() do
  writeln(i);
writeln();

for j in A.cols() do
  writeln(j);
writeln();

if compressRows != checkError {
  for i in A.rows() do
    for (j, a) in A.colsAndVals(i) do
      writeln("A[", (i,j), "] = ", a, " check: ", A[i,j]);
} else {
  for j in A.cols() do
    for (i, a) in A.rowsAndVals(j) do
      writeln("A[", (i,j), "] = ", a, " check: ", A[i,j]);
}
