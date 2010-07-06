module domain_test {
  
  var D = [1..1];

  var corner: [D] real;

  writeln(corner);
  writeln(corner(1));
  writeln(corner == corner(1));
  
} // end module domain_test