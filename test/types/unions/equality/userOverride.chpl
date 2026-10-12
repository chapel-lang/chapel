union u {
  var x: int;
  var y: real;
}

operator ==(u1: u, u2: u) {
  if u1.getActiveIndex() == -1 {
    return u2.getActiveIndex() == -1;
  } else if u2.getActiveIndex() == -1 {
    return false;
  } else if u1.getActiveIndex() == 0 {
    if u2.getActiveIndex() == 0 {
      return u1(0) == u2(0);
    } else {
      return u1(0) == u2(1);
    }
  } else {
    if u2.getActiveIndex() == 0 {
      return u1(1) == u2(0);
    } else {
      return u1(1) == u2(1);
    }
  }
}

operator !=(u1: u, u2: u) {
  return !(u1 == u2);
}
    
var u1, u2: u;

testit();

u1.x = 42;

testit();

u2.x = 42;

testit();

u2.y = 42.0;

testit();

u2.y = 42.1;

testit();

u1.y = 42.1;

testit();

u2.x = 43;

testit();

u1.x = 43;

testit();



proc testit() {
  writeln(u1, " == ", u2, ": ", u1 == u2);
  writeln(u1, " != ", u2, ": ", u1 != u2);
}

