var d1 = [1..2];
var a1: [d1] int; a1[1] = 555;

class C {
  var d;
  var a: [d] int;
}

proc C.init(d, a) where d: domain(?) {
  this.d = d;
  this.a = a;
}

var c1 = new C(d1,a1);
writeln(c1);
