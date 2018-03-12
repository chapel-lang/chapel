// Initialize a class field in a record with an initializer
record Container {
  var y: Stored;
  var v;

  proc init() {
    y = new Stored(true);
    v = 10;
  }

  proc deinit() {
    delete y;
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
