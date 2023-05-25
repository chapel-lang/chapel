class C {
  enum probClass { A, B, C, D, E };

  var x: probClass;
}

var myC = (new owned C()).borrow();

writeln("myC = ", myC);
