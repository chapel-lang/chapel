
record R {
  var x : int;

  proc init(x : int) {
    writeln("R.init(int)");
    this.x = x;
  }

  proc init(other : R) {
    writeln("R.init(R)");
    this.x = other.x;
  }
}

proc main() {
  var A = new R(5);
  var B = A;
  var C = B;
}
