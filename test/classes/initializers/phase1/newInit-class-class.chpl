// Initialize a class field in a class with an initializer
class Container {
  var y: Stored;

  proc init() {
    y = new Stored(true);

    super.init();
  }

  proc deinit() {
    delete y;
  }
}

class Stored {
  var x: bool;

  proc init(xVal) {
    x = xVal;

    super.init();
  }
}

var c = new Container();

writeln(c);

delete c;
