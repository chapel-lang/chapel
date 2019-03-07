// Initialize a class field in a class with an initializer
class Container {
  var y;

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

var c = new unmanaged Container();

writeln(c);

delete c;
