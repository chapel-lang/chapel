record R {
  var x: int;
  proc init(flag: bool) {
    if flag then
      this.x = 1;
  }
}

record S {
  var x: int;
  proc init(param flag: bool) {
    if flag then
      this.x = 1;
  }
}


var r1 = new R(true);
var r2 = new R(false);

var s1 = new S(true);
var s2 = new S(false);

writeln(r1);
writeln(r2);

writeln(s1);
writeln(s2);
