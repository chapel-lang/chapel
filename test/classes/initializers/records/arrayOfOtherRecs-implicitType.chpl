record R {
  var x: int;

  proc init(xVal: int = 0) {
    x = xVal;
    super.init();
  }
}

record StoresR {
  var x: [1..3] R;

  proc init(arr = [new R(2), new R(3), new R(4)]) {
    x = arr;
    super.init();
  }
}

var storing = new StoresR();
writeln(storing);
