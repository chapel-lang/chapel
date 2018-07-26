// Initialize a record field in a class with an initializer
class Container {
  var y;

  proc init() {
    y = new Stored(true);

  }
}

record Stored {
  var x: bool;

  proc init(xVal:bool) {
    x = xVal;
  }
}

var c = new unmanaged Container();

writeln(c);

delete c;
