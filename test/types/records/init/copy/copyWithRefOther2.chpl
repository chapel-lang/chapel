record R {
  var x: int;
  
  proc init(x: int) {
    this.x = x;
  }

  proc init=(ref src: R) {
    this.x = src.x;
    this.complete();
    src.x = 0;
  }
}

var myR = new R(42);
var myR2 = myR;

writeln(myR);
writeln(myR2);
