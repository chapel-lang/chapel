class C {
  type t;

  proc init(type t, n) {
    this.t = t;
    super.init();
  }

  proc init(other: C(?otherType),
            type classType = otherType) {
    this.t = other.t;
    super.init();
  }
}

var myC = new C(real, 10);
var otherC = new C(myC);
writeln(otherC);
writeln(otherC.t:string);

delete otherC, myC;
