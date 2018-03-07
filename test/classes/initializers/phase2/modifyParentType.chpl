class C {
  type x;

  proc init(type t) {
    this.x = t;
    super.init();
  }
}

class D : C {
  proc init(type t) {
    super.init(t);
    this.initDone();
    this.x = int;
  }
}

var myD = new D(real);
writeln(myD);
delete myD;
