// Initialize a class field in a class with an initializer
class Container {
  var y: unmanaged Stored;

  proc init() {
    y = new unmanaged Stored(true);

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

var c = new unmanaged Container();

writeln(c);

delete c;
