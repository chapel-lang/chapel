record R {
  param stridable: bool = false;
  var str: if stridable then int else void;

  proc init(param stridable) where !stridable: R {
    this.stridable = stridable;
    super.init();
    if stridable then
      str = 1;
  }
}

var myR = new R(true);
var myR2 = new R(false);

writeln(myR);
writeln(myR2);
