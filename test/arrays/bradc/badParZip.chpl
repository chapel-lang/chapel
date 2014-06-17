var A: [1..4] int;

writeln("Case 1: Expecting 1 2 3 4");
for x in 1..4 { writeln(x); }
A = 1..4;
writeln(A);

writeln("Case 2: Expecting ");
for x in 4..1 { writeln(x); }
A = 4..1;
writeln(A);

writeln("Case 3: Expecting 4 3 2 1");
for x in 1..4 by -1 { writeln(x); }
A = 1..4 by -1;
writeln(A);

writeln("Case 4: Expecting ");
for x in 4..1 by -1 { writeln(x); }
A = 4..1 by -1;
writeln(A);

writeln("Case 5: Expecting 4 3 2 1");
// for loop correctly gives error for unbounded range
//for x in 4.. by -1 { writeln(x); }
A = 4.. by -1;
writeln(A);


writeln("Case 6: Expecting 4 3 2 1");
// for loop is (correctly) infinite loop.
//for x in ..4 by -1 { writeln(x); }
A = ..4 by -1;
writeln(A);
