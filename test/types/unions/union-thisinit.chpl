union u {
  var x: int;
  var y: int;
  var z: real;

  proc init() {
    this.x = 42;
  }
  
  proc init(x: int) {
    this.init();
    writeln(this); // make sure object is usable
    this.foo();
    this.y = 45;   // change the value, for fun
  }

  proc foo() {
    writeln("In u.foo()");
  }
}

var u1: u;
writeln(u1);
var u2 = new u();
writeln(u2);
var u3 = new u(x=1);
writeln(u3);

