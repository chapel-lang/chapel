proc idamax(dx : [?D] real) where D.rank==1 {
  writeln("In idamax, dx is: ", dx);
}


proc idamax2(dx : [?D]) where D.rank==1 {
  writeln("In idamax2, dx is: ", dx);
}


var A: [1..3] real = (1.0, 2.0, 3.0);

idamax(A);
idamax2(A);
