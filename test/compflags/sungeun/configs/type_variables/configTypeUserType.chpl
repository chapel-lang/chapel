config type myIdxType = int;

record rPair {
  var x: myIdxType;
  var y: myIdxType;
}

class cPair {
  var x: myIdxType;
  var y: myIdxType;
}

config type myType = rPair;
var p: toNilableIfClassType(myType);

writeln("p = ", p);
writeln("numBits(myIdxType) = ", numBits(myIdxType));
