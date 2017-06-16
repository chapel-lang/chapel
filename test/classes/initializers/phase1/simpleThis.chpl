record MyRecord {
  var x : int = 1;
  var y : int = 2;


  proc init(xVal) {
    x = xVal;
    y = 1 + this.x;

    super.init();
  }
}

proc main() {
  var r : MyRecord = new MyRecord(10);

  writeln(r);
}
