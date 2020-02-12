
record R {
  var x : int;

  proc chpl__promotionType() type {
    return int;
  }

  proc init(x : int) {
    this.x = x;
  }

  proc init(other:R) {
    writeln("Copying from R(", other.x, ")");
    this.x = other.x * 2;
  }

  proc init=(other:R) {
    writeln("Copying from R(", other.x, ")");
    this.x = other.x * 2;
  }

  iter these() {
    for i in 1..10 do yield i*x;
  }
}

proc main() {
  var r = new R(2);
  writeln("1 + r = ", 1 + r);

  var o = new R(r);
  writeln("2 * o = ", 2 * o);

  var b = r;
  writeln("3 * b = ", 3 * b);
}
