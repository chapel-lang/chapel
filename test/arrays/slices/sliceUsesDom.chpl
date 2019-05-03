use BlockDist;

var D = {1..9, 1..9};
var SubD = {2..4, 2..4};

var DB = newBlockDom(D);
var SubDB = DB[SubD];

testit(D);
testit(DB);

proc testit(dom) {
  var A: [dom] real = [(i,j) in D] i + j/10.0;

  ref AD = A[SubD];
  ref ADB = A[SubDB];

  writeln(AD, "\n");
  writeln(ADB, "\n");

  // Show that changing the domain changes the view
  SubD = SubD.translate(1,1);
  SubDB = SubDB.translate(1,1);

  writeln(AD, "\n");
  writeln(ADB, "\n");
}

