record R {
  var x: int;
  proc init(in x: int = 45) {
    writeln("In init");
    this.x = x;
  }
  proc init=(lhs: R) {
    writeln("In init=");
    this.x = lhs.x;
  }
  operator =(ref lhs: R, rhs: R) {
    writeln("In assign");
    lhs.x = rhs.x;
  }
  proc deinit() {
    writeln("In deinit");
  }
}

union u {
  var r1: R;
  var r2: R;
  var r3: R;

  proc init() {
    init this;
    writeln("In u.init");
    this.r1 = new R();
    writeln("After r1");
    this.r3 = new R(78);
    writeln("After r3");
    this.r2 = new R(33);
    writeln("Before return");
  }
}

var myU: u;
writeln(myU);
