
record R {
  type T;
  var x : T;

  proc init=(other: int) {
    this.T = (int, int);
    this.x = (other, other);
  }
}

proc main() {
  var r : R(int) = 5;
  writeln(r);
  writeln(r.type:string);
}
