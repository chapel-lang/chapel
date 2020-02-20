config type myIdxType = int;

record rPair {
  var x: myIdxType;
  var y: myIdxType;
}

class cPair {
  var x: myIdxType;
  var y: myIdxType;
}

proc toNonNilableIfClassType(type arg) type {
  if isClassType(arg) then return arg?;
  else                     return arg;  // #14920
}

config type myType = rPair;
var p: toNonNilableIfClassType(myType);

writeln("p = ", p);
writeln("numBits(myIdxType) = ", numBits(myIdxType));
