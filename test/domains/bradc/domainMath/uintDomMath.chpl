var m: uint(64) = 10;
var n: uint(32) = 10;
var mm: int(64) = m:int(64);
var nn: int(32) = n:int(32);

const D32: domain(2, uint(32)) = {1..n, 1..n};
const D64: domain(2, uint(64)) = {1..m, 1..m};

writeln("D32 is: ", D32);
const D32up = D32.translate(nn,nn);
writeln("D32up is: ", D32up);
const D32dn = D32up.translate(-nn/2, -nn/2);
writeln("D32dn is: ", D32dn);

writeln("D64 is: ", D64);
const D64up = D64.translate(mm,mm);
writeln("D64up is: ", D64up);
const D64dn = D64up.translate(-mm/2, -mm/2);
writeln("D64dn is: ", D64dn);

