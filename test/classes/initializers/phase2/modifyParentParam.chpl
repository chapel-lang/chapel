class C {
  param x: int;

  proc init(param x : int) {
    this.x = x;

  }
}

class D : C {
  proc init(param x : int) {
    super.init(x);
    this.initDone();
    // Error message should say something about phase 1 or 2
    this.x = 42;
  }
}

var myD = new D(10);
writeln(myD);
delete myD;
