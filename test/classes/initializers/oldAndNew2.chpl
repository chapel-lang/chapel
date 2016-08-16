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

var c: badClass = new badClass(); // Should fail
// Can't distinguish between the two initializer styles.
writeln(c);
// Variation on oldAndNew.chpl with constructor taking a different argument list
// than the initializer
