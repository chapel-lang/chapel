// Initialize a class field in a class with an initializer
class Container {
  var y;

  proc init() {
    y = new Stored(true);

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

delete c;
