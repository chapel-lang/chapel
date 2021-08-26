class C { var x = 0; }

config const doPrint = true;

proc debugln(args ...?k) {
  if doPrint then writeln((...args));
}

var counter: atomic int = 0;

proc makeSharedPtr() {
  var val = counter.fetchAdd(1);
  return new shared C(val);
}

record r {
  var ptr = makeSharedPtr();

  proc init() {
    this.complete();
    debugln('default-init ', this);
  }

  proc init(other: r) {
    this.complete();
    debugln(this, ', init from ', other);
  }

  proc init=(rhs: r) {
    this.complete();
    debugln(this, ', init= from ', rhs);
  }

  proc deinit() {
    debugln('deinit ', this);
  }
}

operator r.=(ref lhs: r, const ref rhs: r) {
  debugln(lhs, ' assigned to by ', rhs); 
}

