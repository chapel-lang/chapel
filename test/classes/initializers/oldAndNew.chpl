class badClass {
  var i = 10;

  proc init() {
    writeln("in new initializer");
  }

  proc badClass() {
    writeln("in old constructor");
  }
}

var c: badClass = new badClass(); // Should fail
// Can't distinguish between the two initializer styles.
writeln(c);
