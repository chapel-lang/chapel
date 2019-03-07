class C {
  var x : int;

  proc C(x : int) {
    this.x = x;
  }
}

proc C.C(s : real) {
  this.x = s:int;
}

class D {
  var y : real;

  proc D(y : real) {
    this.y = y;
  }
}

var c = new owned C(1.5);

var d = new owned D(1.5);
