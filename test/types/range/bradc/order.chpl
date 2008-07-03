def doInd(D, i) {
  writeln("D.order(",i,") = ", D.order(i));
}

def doRange(D) {
  doInd(D, D.low-1);
  for i in D {
    doInd(D, i);
  }
  doInd(D, D.high+1);
  writeln();
}

doRange(3..5);
doRange(3..7 by 2);

