class badClass {
  var i = 10;

  proc init() {
    writeln("in new initializer");
  }

  proc badClass(x) {
    i = x;
    writeln("in old constructor");
  }
}

// 'proc badClass' is a regular method, not a constructor
var c: badClass = new badClass();
writeln(c);
