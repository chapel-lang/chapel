class C {
  type t;
  var x: t;
}

var c1: C(int);
var c2: C(real);

writeln(c1.x.type: string);
writeln(c2.x.type: string);
