union u {
  var x: int;
  var y: int;
  var z: real;

  proc postinit() {
    writeln("after initialization, active field is #", this.getActiveIndex());
  }
}

var u1: u;
var u2 = new u();
var u3 = new u(x=1);
var u4 = new u(y=2);
var u5 = new u(3.4);
var u6 = new u(z=5.6);

