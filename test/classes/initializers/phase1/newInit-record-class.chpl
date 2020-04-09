// Initialize a class field in a record with an initializer
record Container {
  var y: owned Stored;

  proc init() {
    y = new owned Stored(true);
  }
}

class Stored {
  var x: bool;

  proc init(xVal) {
    x = xVal;

  }
}

var c = new Container();

writeln(c);
