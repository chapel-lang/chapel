
record R {
  var x : atomic int;

  proc init(val : int) {
    this.x = val;
  }
}

proc main() {
  var r = new R(5);
  writeln(r);
}
