// Initialize a record field in a record with an initializer
record Container {
  var y: Stored;

  proc init() {
    y = new Stored(true);

    super.init();
  }
}

record Stored {
  var x: bool;

  proc init(xVal) where !xVal: Stored {
    x = xVal;

    super.init();
  }
}

var c = new Container();

writeln(c);
