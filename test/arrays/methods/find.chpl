var A = [1, 2, 3, 2, 5];

writeln("A finds:");
writeln(A.find(3));
writeln(A.find(2));
writeln(A.find(0));
writeln(A.find(5));
writeln(A.find(1));
writeln();

var B: [1..3, 1..3] int;
forall (i,j) in {1..3, 1..3} do
  B[i,j] = (i-1)*3 + j;
B[2,1] = 2;

writeln("B finds:");
writeln(B.find(5));
writeln(B.find(3));
writeln(B.find(7));
writeln(B.find(10));
writeln(B.find(2));
writeln();


var C: [1..10 by 2 align 2] int  = A;

writeln("C finds:");
writeln(C.find(3));
writeln(C.find(2));
writeln(C.find(0));
writeln(C.find(5));
writeln(C.find(1));
writeln();
        
var D: [1..6 by 2, 1..6 by 2 align 2] int = B;

writeln("D finds:");
writeln(D.find(5));
writeln(D.find(3));
writeln(D.find(7));
writeln(D.find(10));
writeln(D.find(2));
writeln();


var Ds: sparse subdomain(B.domain);
Ds.add((2,2));
var As: [Ds] real;
As[2,2] = 2.2;
writeln("As finds:");
writeln(As.find(2.2));
writeln(As.find(0.0));
writeln(As.find(1.1));

// make sure that if an early value is explicitly storing the IRV, we
// still return it.
Ds.add((1,1));
Ds.add((1,2));
As[1,1] = 1.1;
As[1,2] = 0.0;
writeln(As.find(0.0));
writeln(As.find(1.1));
writeln(As.find(3.3));

