record R {
  const c: int;
  var v: int;
}

class C  {
  var A: [1..3] R;

  proc init() {
    var myR = new R(c=1, v=2);
    A = [i in 1..3] myR;
  }
}

var myC = new unmanaged C();

writeln(myC);

delete myC;
