record R {
  var x:int;
  proc init() {
    writeln("in R.init");
    this.x = 1;
  }
  proc init=(other: R) {
    writeln("in R.init=");
    this.x = other.x;
  }
}

proc test1() {
  writeln("test1");
  var x = new R();
  {
    var y = x; // should copy elide
    writeln(y);
  }
}
test1();

proc test2() {
  writeln("test2");
  var x = new R();
  local {
    var y = x; // should copy elide
    writeln(y);
  }
}
test2();
