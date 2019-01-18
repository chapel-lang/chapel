config const gflag = true;

class C {
  const x: real;

  proc init(flag: bool) {
    x = if flag then 1.2 else 3.4;
  }
}

var myC1 = new owned C(true);
var myC2 = new owned C(false);
var myC3 = new owned C(gflag);

writeln(myC1);
writeln(myC2);
writeln(myC3);
