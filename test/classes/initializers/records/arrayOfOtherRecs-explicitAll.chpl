record R {
  var x: int;

  proc init(xVal: int = 0) {
    x = xVal;
    this.initDone();
  }
}

record StoresR {
  var x: [1..3] R;

  proc init(arr: [1..3] R = [new R(2), new R(3), new R(4)]) {
    x = arr;
    this.initDone();
  }
}

var storing = new StoresR();
writeln(storing);
