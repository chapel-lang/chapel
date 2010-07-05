module domain_test {
  
  var D = [1..10,2..9];
  
  var offarray: [1..2] int = (4,3);
  var offtuple: 2*int;
  
  offtuple = offarray;

  writeln(offtuple);
  // writeln(D.interior((4,3)));

  
} // end module domain_test