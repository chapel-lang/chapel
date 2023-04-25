class badClass {
  var i = 10;

  proc badClass(x) {
    i = x;
    writeln("in old constructor");
  }

  proc init() {
    writeln("in new initializer");
  }
}

// 'proc badClass' is a regular method, not a constructor
var c: badClass = new badClass(); // Should fail
writeln(c);
