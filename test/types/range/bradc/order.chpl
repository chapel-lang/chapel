proc doInd(D, i) {
  writeln("D.order(",i,") = ", D.indexOrder(i));
}

proc doRange(D) {
  doInd(D, D.lowBound-1);
  for i in D {
    doInd(D, i);
  }
  doInd(D, D.highBound+1);
  writeln();
}

doRange(3..5);
doRange(3..7 by 2);

