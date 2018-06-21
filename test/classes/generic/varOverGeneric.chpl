class C {
  type t;
  var x: t;
}

var myC: borrowed C = new borrowed C(int, 1);
writeln(myC);
