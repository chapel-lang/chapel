// Initialize a record field in a class with an initializer
class Container {
  var y: Stored;

  proc init() {
    y = new Stored(true);

  }
}

record Stored {
  var x: bool;

  proc init(xVal) where !xVal: Stored {
    x = xVal;
  }
}

var c = new Container();

writeln(c);

delete c;
