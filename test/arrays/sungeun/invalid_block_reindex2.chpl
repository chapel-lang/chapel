config var n: uint = 1;
const lo: int(64) = -1;
const hi: int(64) = 1;

use BlockDist;

var D0: domain (1,int(64)) = [min(int(64))..min(int(64))+n];
var D1 = [min(int(64))..min(int(64))+n] distributed Block(rank=1,
                                                          idxType=int(64),
                                                          bbox=[lo..hi]);
var D2: domain (1,int(64)) = [max(int(64))-n..max(int(64))];

def foo(D: domain, A:[D], i) {
  A += n:int*i;
}

var A0: [D0] int;
A0 = 1;
// This reindex works for the default distribution
foo(D2, A0, 2);

var A1: [D1] int;
A1 = 1;
// This reindex does not work for the Block distribution
foo(D2, A1, 2);
writeln("We should not get here.");
