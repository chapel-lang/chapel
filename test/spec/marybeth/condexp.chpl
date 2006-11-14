writehalf(8);
writehalf(21);
writehalf(1000);

def writehalf(i: int) {
  var half = if (i % 2) then i/2 +1 else i/2;
  writeln("Half of ",i," is ",half); 
}


