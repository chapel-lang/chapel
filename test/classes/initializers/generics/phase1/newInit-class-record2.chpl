// Initialize a record field in a class with an initializer
class Container {
  var y;

  proc init() {
    y = new Stored(true);

    super.init();
  }
}

record Stored {
  var x: bool;

  proc init(xVal:bool) {
    x = xVal;

    super.init();
  }
}

var c = new Container();

writeln(c);

delete c;
