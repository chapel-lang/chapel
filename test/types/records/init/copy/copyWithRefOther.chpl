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

proc callsCopy(const ref arg) {
  return arg; // compiler adds copy-init call here
              // but it should be an error since arg is const.
}

const myR = new R(42);
var myR2 = callsCopy(myR);

writeln(myR);
writeln(myR2);
