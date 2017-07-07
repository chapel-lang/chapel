config const gflag = true;

class C {
  const x: real;

  proc init(flag: bool) {
    x = if flag then 1.2 else 3.4;
    super.init();
  }
}

var myC1 = new C(true);
var myC2 = new C(false);
var myC3 = new C(gflag);

writeln(myC1);
writeln(myC2);
writeln(myC3);

delete myC1;
delete myC2;
delete myC3;
