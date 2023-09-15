class C { var x = 0; }

config const doPrint = true;

proc debugln(args ...?k) {
  if doPrint then writeln((...args));
}

record r {
  var ptr = new shared C();

  proc init() {
    init this;
    debugln('default-init: ', this);
  }

  proc init(other: r) {
    this.ptr = other.ptr;
    init this;
    debugln('init-r: ', this);
  }

  proc init(num: int) {
    this.ptr = new shared C(num);
    init this;
    debugln('init-int: ', this);
  }

  proc init=(rhs: r) {
    this.ptr = rhs.ptr;
    init this;
    debugln('init=: ', this);
  }

  proc deinit() {
    debugln('deinit: ', this);
  }
}

operator r.=(ref lhs: r, const ref rhs: r) {
  lhs.ptr = rhs.ptr;
  debugln('assignment: ', lhs);
}

