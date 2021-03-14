var i8 = 1:int(8);
var j8 = 3:int(8);

var r = i8..j8;

writeln("r.idxType = ", r.idxType:string);

for i in r do
  writeln("i: ", i, "(", i.type:string, ")");

var D = {r, r};

writeln("D.idxType = ", D.idxType:string);

for ij in D do
  writeln("ij: ", ij, "(", ij(0).type:string, ",", ij(1).type:string, ")");
