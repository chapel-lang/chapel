record R {
  var x: int;

  proc init(xVal: int = 0) {
    x = xVal;
    this.initDone();
  }
}

record StoresR {
  var x: [1..3] R;
}

var storing = new StoresR();
writeln(storing);
