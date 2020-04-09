// Initialize a record field in a class with an initializer
class Container {
  var y: Stored;
  var v;

  proc init() {
    y = new Stored(true);
    v = 10;

  }
}

record Stored {
  var x: bool;

  proc init(xVal) where !isSubtype(xVal.type, Stored) {
    x = xVal;
  }
}

var c = new owned Container();

writeln(c);
