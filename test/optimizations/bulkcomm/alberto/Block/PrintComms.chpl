use CommDiagnostics;

proc myPrintComms(mesg:string) {
  const A=getCommDiagnostics();
  var i= +reduce A.get + +reduce A.put;
  write(mesg,"Total g+p:",i," ");
  for a in A do write(" (",a.get,"g ", a.put,"p ",a.get_nb+a.get_nb_test+a.get_nb_wait,"og ", a.fork+a.fork_fast+a.fork_nb, "f)");
  writeln();
  //var i,j=0;
  //i = + reduce A.get;
  //j = + reduce A.put;
  //writeln("Total gets=",i,"; Total puts=",j);
}

