class C {
  var x: int;
}

var myC = new C();

begin with (in myC) {
  writeln(myC.x);
}

writeln(myC.x);
