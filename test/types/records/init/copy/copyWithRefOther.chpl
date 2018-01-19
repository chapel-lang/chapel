record R {
  var x: int;
  
  proc init(x: int) {
    this.x = x;
    super.init();
  }

  proc init(ref src: R) {
    this.x = src.x;
    super.init();
    src.x = 0;
  }
}

var myR = new R(42);
var myR2 = myR;

writeln(myR);
writeln(myR2);
