/* This example demonstrates how to use formatted I/O to
   write and read a record stored as a tab-separated
   fields, with one record per line.
 */

/* MyRecord contains fields of various types in order to demonstrate
   functionality.
 */
record MyRecord {
  var i: int;
  var j: int;
  var x: real;
  var y: real;
  var a: string;
  var b: string;
}

config const fileName = "test.txt";

// Let's create a few records and store them in an array.
var A = [ new MyRecord(1,2,3.0,4.0,"test one", "test two"),
          new MyRecord(6,7,-1.1,-2.2,"quick brown", "fox jumped") ];

writeln(A);

writef("%ht\n", A);
writef("%jt\n", A);


var B:[1..9, 1..9] int;

for (i,j) in B.domain {
  B[i,j] = 100*i + j;
}

writeln(B);

writef("%ht\n", B);
writef("%jt\n", B);

var C:[1..3, 1..3, 1..3] int;

for (i,j,k) in C.domain {
  C[i,j,k] = 100*i + 10*j + k;
}

writeln(C);

writef("%ht\n", C);
writef("%jt\n", C);

var D:[1..2, 1..2, 1..2, 1..2] int;

for (i,j,k,l) in D.domain {
  D[i,j,k,l] = 1000*i + 100*j + 10*k + l;
}

writeln(D);

writef("%ht\n", D);
writef("%jt\n", D);


