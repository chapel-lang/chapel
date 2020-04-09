record R {
  var x: int;

  proc init(x: int) {
    this.x = x;
  }

  proc init=(const ref r: R) {
    this.x = r.x;
  }
}

var myR = new R(42);
var myR2 = myR;
writeln(myR2);
