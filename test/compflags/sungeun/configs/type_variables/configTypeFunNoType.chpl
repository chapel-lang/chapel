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

proc toNonNilableIfClassType(type arg) type {
  if isClassType(arg) then return arg?;
  else                     return arg;  // #14920
}

proc f(p) {
  writeln("p = ", p);
}

var p: toNonNilableIfClassType(myType);

f(p);
writeln("numBits(myIdxType) = ", numBits(myIdxType));
