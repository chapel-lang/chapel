// This is like test_composite1 except the array is originally over
// the indices [10001..10005, 10001..10005, 10001..10005]

use driver;

var MyA3D: [{10001..10005, 10001..10005, 10001..10005} dmapped Dist3D] int;

var S: [1..n3, 1..n3] string;

proc writeToString() {
  for (i,j,k) in Space3 do
    S(i,j) += MyA3D(10000+i,10000+j,10000+k):string;
  for s in S do
    s += "  ";
}

ref AA = MyA3D;                    // alias [1..5, 1..5, 1..5]
[aa in AA] aa = 1;
writeToString();

ref AA2 = AA({10001..10005, 10001..10005, 10001..10005}.expand(-1));   // slice [2..4, 2..4, 2..4]
[aa2 in AA2] aa2 = 2;
writeToString();

ref AA3 = AA2.reindex(91..93, 94..96, 97..99); // reindex [2..4, 2..4, 2..4]
[aa3 in AA3] aa3 = 3;
writeToString();

ref AA4 = AA3(92, 94..96, 97..99);      // collapse [3, 2..4, 2..4]
[aa4 in AA4] aa4 = 4;
writeToString();

ref AA5 = AA4;                     // alias [3, 2..4, 2..4]
[aa5 in AA5] aa5 = 5;
writeToString();

ref AA6 = AA5(95..96, 98..99);         // slice [3, 3..4, 3..4]
[aa6 in AA6] aa6 = 6;
writeToString();

ref AA7 = AA6.reindex(1..2, 1..2);     // reindex [3, 3..4, 3..4]
[aa7 in AA7] aa7 = 7;
writeToString();

ref AA8 = AA7(1..2, 2);          // collapse [3, 3..4, 4]
[aa8 in AA8] aa8 = 8;
writeToString();

AA8(1) = 9;                        // index [3, 3, 4]
writeToString();

for i in 1..n3 {
  for j in 1..n3 {
    writeln(S(i,j));
  }
  writeln();
}
