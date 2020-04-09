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

proc f(p: toNilableIfClassType(myType)) {
  writeln("p = ", p);
}

var p: toNilableIfClassType(myType);

f(p);
writeln("numBits(myIdxType) = ", numBits(myIdxType));
