class C {
  type x;

  proc init(type t) {
    this.x = t;

  }
}

class D : C(?) {
  proc init(type t) {
    super.init(t);
    init this;
    this.x = int;
  }
}

var myD = new D(real);
writeln(myD);
