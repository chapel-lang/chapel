proc foo() {
   var A: [1..3] [1..3] real;
   forall (i,j) in {1..3, 1..3} do
       A[i][j] = i + j/10.0;

   ref myA = A[2];
   writeln(myA);
   writeln(myA[2]);
}

foo();

