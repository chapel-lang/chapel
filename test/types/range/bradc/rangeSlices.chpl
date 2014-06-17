config const n = 5;

const R = 1..n;

writeln("R is: ", R);
writeln("R(2..n-1) is: ", R(2..n-1));
writeln("R(2..) is: ", R(2..));
writeln("R(..n-1) is: ", R(..n-1));
writeln("R(..) is: ", R(..));
