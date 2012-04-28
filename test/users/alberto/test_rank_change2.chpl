var A: [1..5, 1..5] int = 1..;
var F: [1..5, 1..5] int = 1..;

var B: [1..5] int;

var C: [1..5,1..5,1..5] int = 100..;

var E: [1..5,1..5,1..5] int = 500..;

var D: [1..5,1..5,1..5,1..5] int = 1000..;
writeln("A:",A);
writeln("B:",B);
writeln("C:",C);
writeln("D: ",D);

writeln("Example 1");
B = A[1..5, 1];
writeln("A[1..5,1]:",B);

writeln();
writeln("=====================================");
writeln("Example 2");
B = A[1, 1..5];
writeln("A[1,1..5]:",B);

writeln();
writeln("=====================================");
writeln("Example 3");
B=C[1,1,1..5];
writeln("C[1,1,1..5]:", B);

writeln();
writeln("=====================================");
writeln("Example 4");
B=C[1,1..5,1];
writeln("C[1,1..5,2]:", B);

writeln();
writeln("=====================================");
writeln("Example 5");
B=C[1..5,1,1];
writeln("C[1..5,1,1]:", B);

writeln();
writeln("=====================================");
writeln("Example 6");
B=D[1,1,1,1..5];
writeln("D[1,1,1,1..5]:", B);

writeln();
writeln("=====================================");
writeln("Example 7");
B=D[1,1,1..5,1];
writeln("D[1,1,1..5,1]:", B);

writeln();
writeln("=====================================");
writeln("Example 8");
B=D[1,1..5,1,1];
writeln("D[1,1..5,1,1]:", B);

writeln();
writeln("=====================================");
writeln("Example 9");
B=D[1..5,1,1,1];
writeln("D[1..5,1,1,1]:", B);


writeln();
writeln("=====================================");
writeln("Example 10");
F=C[1,1..5,1..5];
writeln("C[1,1..5,1..5]:", F);

writeln();
writeln("=====================================");
writeln("Example 11");
F=C[1..5,1,1..5];
writeln("C[1..5,1,1..5]:", F);

writeln();
writeln("=====================================");
writeln("Example 12");
F=C[1..5,1..5,1];
writeln("C[1..5,1..5,1]:", F);

writeln();
writeln("=====================================");
writeln("Example 13");
E=C[1..1,1..5,1..5];
writeln("C[1..1,1..5,1..5]:", E);

writeln();
writeln("=====================================");
writeln("Example 14");
E=C[1..5,1..1,1..5];
writeln("C[1..5,1..1,1..5]:", E);

writeln();
writeln("=====================================");
writeln("Example 15");
E=C[1..5,1..5,1..1];
writeln("C[1..5,1..5,1..1]:", E);


//proc foo(B: [] int) {
//  writeln(B);
//}

//foo(A[1, 1..5]);
//foo(A[1..5, 1]);
