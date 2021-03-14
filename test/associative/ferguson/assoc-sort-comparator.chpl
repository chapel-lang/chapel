use Sort;

var D:domain(int);

var A:[D] int;


D += 1;
D += 2;
D += 3;
D += 5;
D += 7;


A[1] = 10;
A[2] = 8;
A[3] = 7;
A[5] = 5;
A[7] = 3;

writeln("reverse sorted D");
for i in D.sorted(reverseComparator) {
  writeln(i);
}

writeln("reverse sorted A");
for x in A.sorted(reverseComparator) {
  writeln(x);
}


