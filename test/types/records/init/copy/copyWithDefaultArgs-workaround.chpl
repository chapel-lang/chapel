record R {
  var x: int;

  proc init(x: int, y: bool = true) {
    this.complete();
    if y then
      this.x = x;
  }

  proc init(r: R, y: bool) {
    this.complete();
    if y then
      this.x = r.x;
  }

  proc init(r: R) {
    this.init(r, true);
  }
}

var myR = new R(1, true),
    myR2 = new R(2, false),
    myR3 = new R(3);

writeln(myR);
writeln(myR2);
writeln(myR3);

var myManualCopy = new R(myR3, true);
writeln(myManualCopy);

var myCopy = myR;
writeln(myCopy);
