config type myIdxType = int;

class C {
  type t;
  var x, y: t;
}

var p = new C(myIdxType);

writeln("p = ", p);
writeln("numBits(myIdxType) = ", numBits(myIdxType));
