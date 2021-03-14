record R {
  param stridable: bool = false;
  var str = if stridable then 1 else none;

  proc init(param stridable) {
    this.stridable = stridable;
  }
}

var myR = new R(true);
var myR2 = new R(false);

writeln(myR);
writeln(myR2);
