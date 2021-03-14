// Initialize a record field in a record with an initializer
record Container {
  var y: Stored;

  proc init() {
    y = new Stored(true);

  }
}

record Stored {
  var x: bool;

  proc init(xVal) where !isSubtype(xVal.type, Stored) {
    x = xVal;

  }
}

var c = new Container();

writeln(c);
