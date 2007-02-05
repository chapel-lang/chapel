config var n: int = 8;

var d2: domain(2) distributed(Block) = [1..n, 1..n];

writeln(d2);
