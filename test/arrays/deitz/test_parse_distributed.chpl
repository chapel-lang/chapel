config var n: int = 8;

var d1: domain(1) distributed = [1..n];
var d2: domain(2) distributed(Block) = [1..n, 1..n];
var d3: domain(2) distributed(Block(n,n)) = [1..n, 1..n];

writeln(d1, d2, d3);
