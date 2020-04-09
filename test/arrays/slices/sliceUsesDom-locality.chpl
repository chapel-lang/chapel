use BlockDist;

var D = {1..9, 1..9};
var SubD = {4..6, 4..6};

var DB = newBlockDom(D);
var SubDB = DB[SubD];

testit(D, SubD);
testit(D, SubDB);
testit(DB, SubD);
testit(DB, SubDB);

proc testit(dom, subdom) {
  var A: [dom] real = [(i,j) in D] i + j/10.0;

  forall a in A[subdom] do
    a = here.id;

  writeln(A, "\n");
}
