record R {
  var x: int;

  proc init(x: int, y: bool = true) {
    this.initDone();
    if y then
      this.x = x;
  }

  proc init(r: R, y: bool = true) {
    this.initDone();
    if y then
      this.x = r.x;
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
