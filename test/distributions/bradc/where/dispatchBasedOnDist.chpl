use BlockDist, CyclicDist;

var DBlk: domain(1) dmapped Block(boundingBox={1..10}) = {1..10};
var DCyc: domain(1) dmapped Cyclic(startIdx=1) = {1..10};

var ABlk: [DBlk] real;
var ACyc: [DCyc] real;

proc domproc(D: domain) where D.dist: Block {
  writeln("In the domproc() for Block");
}

proc domproc(D: domain) where D.dist: Cyclic {
  writeln("In the domproc() for Cyclic");
}

proc arrproc(A: []) where A.domain.dist: Block {
  writeln("In the arrproc() for Block");
}

proc arrproc(A: []) where A.domain.dist: Cyclic {
  writeln("In the arrproc() for Cyclic");
}

domproc(DBlk);
domproc(DCyc);

arrproc(ABlk);
arrproc(ACyc);
