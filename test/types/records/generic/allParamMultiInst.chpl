record R {
  param x: int;
  param y: int;

  proc writeThis(c) {
    c.write(x,".",y);
  }
}

record S {
  var r: R(?);
  param z: int;

  proc init(param x: int, param y: int, param z: int) {
    this.r = new R(x, y);
    this.z = z;
  }

  proc writeThis(c) {
    c.write(r);
    c.write(".",z);
  }
}

proc makeS(param x, param y, param z) {
  return new S(x, y, z);
}

var myS1 = makeS(1, 2, 3);
var myS2 = makeS(4, 5, 6);

writeln(myS1);
writeln(myS2);
