
record R {
  var x : atomic int;

  proc init(val : int) {
    this.x = val;
  }
  proc init=(other: R) {
    this.x = other.x.read();
  }
}

proc main() {
  var r = new R(5);
  writeln(r);
}
