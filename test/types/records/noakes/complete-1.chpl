record MyRecord {
  var x : int = 10;
  var y : int = 20;

  proc init(val : int) {
    y = val;

    complete();

    y = y + 10;
  }
}

proc main() {
  var r : MyRecord = new MyRecord(50);

  writeln(r);
}
