use ReplicatedDist;

class C { var x = 0; }

proc C.deinit() {
  writeln('C deinit ' + this:string);
}

record r {
  var ptr = new shared C(1);

  proc init() {
    this.complete();
    writeln('default init ' + this:string);
  }

  proc init(const ref other: r) {
    ptr = other.ptr;
    ptr.x += 1;
    writeln('init copy ' + other:string);
  }

  proc init=(const ref rhs: r) {
    ptr = rhs.ptr;
    ptr.x += 1;
    writeln('init= ' + rhs:string);
  }

  proc deinit() {
    ptr.x -= 1;
    writeln('deinit ' + this:string);
  }
}

proc =(ref lhs: r, const ref rhs: r) {
  writeln('assigning ' + lhs:string + ' to ' + rhs:string);
  lhs.ptr.x -= 1;
  lhs.ptr = rhs.ptr;
  rhs.ptr.x += 1;
}

var d1 = {0..0} dmapped Replicated();
var a1: [d1] r;

proc test() {
  var b: [a1.domain] r = a1;
}
test();

