class C {
  type t;

  proc init(type t, n) {
    this.t = t;
  }

  proc init(other: borrowed C(?otherType),
            type classType = otherType) {
    this.t = other.t;
  }
}

var myC = new unmanaged C(real, 10);
var otherC = new unmanaged C(myC);
writeln(otherC);
writeln(otherC.t:string);

delete otherC, myC;
