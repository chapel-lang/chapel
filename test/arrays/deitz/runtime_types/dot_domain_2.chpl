proc foo(A: [?D] ?T) {
  writeln("A: ", A);
  writeln("D: ", D);
  writeln("T: ", T:string);
  writeln("T.domain: ", T.domain);
}

foo([[1,2,3],[4,5,6]]);
