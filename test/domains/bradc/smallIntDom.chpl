var i8 = 1:int(8);
var j8 = 3:int(8);

var r = i8..j8;

writeln("r.idxType = ", typeToString(r.idxType));

for i in r do
  writeln("i: ", i, "(", typeToString(i.type), ")");

var D = {r, r};

writeln("D.idxType = ", typeToString(D.idxType));

for ij in D do
  writeln("ij: ", ij, "(", typeToString(ij(1).type), ",", typeToString(ij(2).type), ")");
