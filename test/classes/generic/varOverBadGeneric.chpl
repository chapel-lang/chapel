class C {
  type t;
  var x: t;
}

var myC: borrowed C;
myC = new borrowed C();
writeln(myC);
