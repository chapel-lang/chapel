record MyRecord {
  param x : int = 1;
  var y : int = 2;


  proc init(param xVal) where !isSubtype(xVal.type, MyRecord) {
    x = xVal;
    y = 1 + this.x;

  }
}

proc main() {
  var r : MyRecord(10) = new MyRecord(10);

  writeln(r);
}
