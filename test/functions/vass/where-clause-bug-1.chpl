var d1 = [1..2];
var a1: [d1] int; a1[1] = 555;

class C {
  var d;
  var a: [d] int;
}

proc C.C(d, a) where d: domain(?) { }   // btw this does transfer a-the-arg to a-the-field

var c1 = new C(d1,a1);
writeln(c1);
