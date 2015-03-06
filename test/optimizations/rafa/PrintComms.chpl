use CommDiagnostics;

proc myPrintComms(mesg:string) {
  const A=getCommDiagnostics();
  var i= +reduce A.get + +reduce A.get_nb + +reduce A.put + +reduce A.put_nb;
  write(mesg,"Total g+p:",i," ");
  for a in A do write(" (",a.get+a.get_nb,"g ", a.put+a.put_nb,"p ",a.test_nb+a.wait_nb+a.try_nb,"nbt ", a.fork+a.fork_fast+a.fork_nb, "f)");
  writeln();
  //var i,j=0;
  //i = + reduce A.get + + reduce A.get_nb;
  //j = + reduce A.put + + reduce A.put_nb;
  //writeln("Total gets=",i,"; Total puts=",j);
}
