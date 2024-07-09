record R {
  param stridable: bool = false;
  var str: if stridable then int else nothing;

  proc init(param stridable) {
    this.stridable = stridable;
    init this;
    if stridable then
      str = 1;
  }
}

var myR = new R(true);
var myR2 = new R(false);

writeln(myR);
writeln(myR2);
