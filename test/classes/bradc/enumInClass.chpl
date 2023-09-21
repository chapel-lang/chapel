class C {
  enum probClass { A, B, C, D, E };

  var x: probClass;
}

var ownMyC = new owned C();
var myC = ownMyC.borrow();

writeln("myC = ", myC);
