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
}

operator =(ref lhs: R, rhs: R) {
  writeln("In assign");
  lhs.x = rhs.x;
}
    
union u {
  var r1: R;
  var r2: R;
  var r3: R;

  proc init() {
    writeln("In u.init");
    this.r2 = new R(33);
    writeln("Before return");
  }
}

var myU: u;
writeln(myU);
