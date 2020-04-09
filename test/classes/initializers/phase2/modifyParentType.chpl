class C {
  type x;

  proc init(type t) {
    this.x = t;

  }
}

class D : C {
  proc init(type t) {
    super.init(t);
    this.complete();
    this.x = int;
  }
}

var myD = new borrowed D(real);
writeln(myD);
