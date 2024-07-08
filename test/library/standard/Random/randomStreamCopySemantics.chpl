use Random;

var rs1 = new randomStream(int, 123);

// take a few random numbers from the stream:
writeln(rs1.next());
writeln(rs1.next());
writeln(rs1.next());

// copy rs1 to a new stream:
var rs2 = rs1;

// ensure both streams are in the same state:
writeln(rs1.seed, "\t", rs2.seed);
writeln(rs1.next(), "\t", rs2.next());
