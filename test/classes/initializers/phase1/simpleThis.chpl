record MyRecord {
  var x : int = 1;
  var y : int = 2;


  proc init(xVal) where !isSubtype(xVal.type, MyRecord) {
    x = xVal;
    y = 1 + this.x;

  }
}

proc main() {
  var r : MyRecord = new MyRecord(10);

  writeln(r);
}
