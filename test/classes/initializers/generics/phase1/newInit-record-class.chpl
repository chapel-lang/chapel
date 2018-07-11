// Initialize a class field in a record with an initializer
record Container {
  var y: unmanaged Stored;
  var v;

  proc init() {
    y = new unmanaged Stored(true);
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
