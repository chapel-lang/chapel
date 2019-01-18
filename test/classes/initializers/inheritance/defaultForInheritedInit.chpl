class C {
  var x: int;
  var y: string;
  var z: real;

  proc init(x: int, y: string, z: real) {
    this.x = x;
    this.y = y;
    this.z = z;

  }
}

class D : C {
}

var myD = new unmanaged D(3, "hi", 4.6);
writeln(myD);
delete myD;
