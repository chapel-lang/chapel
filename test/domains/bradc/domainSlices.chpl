config const n = 5;

const D = {1..n};

writeln("D is: ", D);
writeln("D[2..n-1] is: ", D[2..n-1]);
writeln("D[2..] is: ", D[2..]);
writeln("D[..n-1] is: ", D[..n-1]);
writeln("D[..] is: ", D[..]);
