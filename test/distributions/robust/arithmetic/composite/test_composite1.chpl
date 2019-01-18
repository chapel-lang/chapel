use driver_arrays;

var S: [1..n3, 1..n3] string;

proc writeToString() {
  for (i,j,k) in Space3 do
    S(i,j) += A3D(i,j,k):string;
  for s in S do
    s += "  ";
}

ref AA = A3D;                      // alias A3D[1..5, 1..5, 1..5]
[aa in AA] aa = 1;
writeToString();

ref AA2 = AA(Space3.expand(-1));   // slice A3D[2..4, 2..4, 2..4]
[aa2 in AA2] aa2 = 2;
writeToString();

ref AA3 = AA2.reindex(1..3, 4..6, 7..9); // reindex A3D[2..4, 2..4, 2..4]
[aa3 in AA3] aa3 = 3;
writeToString();

ref AA4 = AA3(2, 4..6, 7..9);      // collapse A3D[3, 2..4, 2..4]
[aa4 in AA4] aa4 = 4;
writeToString();

ref AA5 = AA4;                     // alias A3D[3, 2..4, 2..4]
[aa5 in AA5] aa5 = 5;
writeToString();

ref AA6 = AA5(5..6, 8..9);         // slice A3D[3, 3..4, 3..4]
[aa6 in AA6] aa6 = 6;
writeToString();

ref AA7 = AA6.reindex(10..11, 1..2);     // reindex A3D[3, 3..4, 3..4]
[aa7 in AA7] aa7 = 7;
writeToString();

ref AA8 = AA7(10..11, 2);          // collapse A3D[3, 3..4, 4]
[aa8 in AA8] aa8 = 8;
writeToString();

AA8(10) = 9;                        // index A3D[3, 3, 4]
writeToString();

for i in 1..n3 {
  for j in 1..n3 {
    writeln(S(i,j));
  }
  writeln();
}
