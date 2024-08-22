use ReplicatedDist;

class C { var x = 0; }

proc C.deinit() {
  writeln('C deinit ', this);
}

record r {
  var ptr = new shared C(1);

  proc init() {
    init this;
    writeln('default init ', this);
  }

  proc init(const ref other: r) {
    ptr = other.ptr;
    ptr.x += 1;
    writeln('init copy ', other);
  }

  proc init=(const ref rhs: r) {
    ptr = rhs.ptr;
    ptr.x += 1;
    writeln('init= ', rhs);
  }

  proc deinit() {
    ptr.x -= 1;
    writeln('deinit ', this);
  }
}

operator r.=(ref lhs: r, const ref rhs: r) {
  writeln('assigning ' + lhs + ' to ' + rhs);
  lhs.ptr.x -= 1;
  lhs.ptr = rhs.ptr;
  rhs.ptr.x += 1;
}

var d1 = {0..0} dmapped new replicatedDist();
var a1: [d1] r;

proc test() {
  var b: [a1.domain] r = a1;
}
test();
