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

var c: badClass = new badClass(); // Should fail
// Can't distinguish between the two initializer styles.
writeln(c);
// Variation on oldAndNew2.chpl with the constructor occurring before the
// initializer in code.
