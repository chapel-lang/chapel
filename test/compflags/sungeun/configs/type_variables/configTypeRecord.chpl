config type myIdxType = int;

record R {
  type t;
  var x, y: t;
}

var p: R(myIdxType);

writeln("p = ", p);
writeln("numBits(myIdxType) = ", numBits(myIdxType));
