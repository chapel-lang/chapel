// Test bit-wise AND, OR, and XOR logical reductions

param M = 2;

var D: domain(1) = {1..M};
var B: [D] uint(64);

B(1) = 0;
B(2) = 0;
writeln( "\nB[D] = ", B);
writeln( "& reduce B[D] = ", & reduce B);
writeln( "| reduce B[D] = ", | reduce B);
writeln( "^ reduce B[D] = ", ^ reduce B);

B(1) = 0;
B(2) = max( uint(64));
writeln( "\nB[D] = ", B);
writeln( "& reduce B[D] = ", & reduce B);
writeln( "| reduce B[D] = ", | reduce B);
writeln( "^ reduce B[D] = ", ^ reduce B);

B(1) = max( uint(64));
B(2) = 0;
writeln( "\nB[D] = ", B);
writeln( "& reduce B[D] = ", & reduce B);
writeln( "| reduce B[D] = ", | reduce B);
writeln( "^ reduce B[D] = ", ^ reduce B);

B(1) = max( uint(64));
B(2) = max( uint(64));
writeln( "\nB[D] = ", B);
writeln( "& reduce B[D] = ", & reduce B);
writeln( "| reduce B[D] = ", | reduce B);
writeln( "^ reduce B[D] = ", ^ reduce B);

B(1) = 0;
B(2) = 4294967295;
writeln( "\nB[D] = ", B);
writeln( "& reduce B[D] = ", & reduce B);
writeln( "| reduce B[D] = ", | reduce B);
writeln( "^ reduce B[D] = ", ^ reduce B);

B(1) = 4294967295;
B(2) = 0;
writeln( "\nB[D] = ", B);
writeln( "& reduce B[D] = ", & reduce B);
writeln( "| reduce B[D] = ", | reduce B);
writeln( "^ reduce B[D] = ", ^ reduce B);

B(1) = 4294967295;
B(2) = 4294967295;
writeln( "\nB[D] = ", B);
writeln( "& reduce B[D] = ", & reduce B);
writeln( "| reduce B[D] = ", | reduce B);
writeln( "^ reduce B[D] = ", ^ reduce B);

B(1) = 10;
B(2) = 5;
writeln( "\nB[D] = ", B);
writeln( "& reduce B[D] = ", & reduce B);
writeln( "| reduce B[D] = ", | reduce B);
writeln( "^ reduce B[D] = ", ^ reduce B);
