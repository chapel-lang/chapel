class badClass {
  var i = 10;

  proc init() {
    writeln("in new initializer");
  }

  proc badClass() {
    writeln("in old constructor");
  }
}
// 'proc badClass' is a regular method, this uses 'init'.
var c: badClass = new badClass();
writeln(c);
