class C {
  var x: int;
  var y: string;
  var z: real;

  proc init(x, y, z) {
    this.x = x;
    this.y = y;
    this.z = z;

  }
}

class D : C {
  proc init(a, b, c) {
    super.init(a, b, c);
  }
}

var myD = new owned D(x=3, "hi", 4.6);
writeln(myD);
