
proc show(ref A, ref reA) {
  writeln(A.domain);
  writeln(A);
  writeln(reA.domain);
  writeln(reA);
}
{
          var A: [1..10] int;
          const D = {6..15};
          ref reA = A.reindex(D);
          reA[6] = 1; // updates A[1]
          show(A,reA);
}
{
          var A: [3..4, 5..6] int;
          ref reA = A.reindex(13..14, 15..16);
          reA[13,15] = 1; // updates A[3,5]
          show(A,reA);
}
