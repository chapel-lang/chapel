// Initialize a record field in a record with an initializer
record Container {
  var y = new Stored(false);

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

var c = new Container();

writeln(c);
