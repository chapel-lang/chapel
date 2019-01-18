
record R {
  var x : int;
  var y : real;

  proc init(b : bool) {
    if b {
      this.x = 5;
      this.y = 1.0;
    } else {
      this.x = -5;
      this.y = 1.0;
    }
    this.x *= 2;
    this.complete();
  }
}

var F = new R(false);
var T = new R(true);

writeln("F = ", F);
writeln("T = ", T);
