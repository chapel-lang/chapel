var myTurn$: [LocaleSpace] sync bool;
myTurn$(0).writeEF(true);


proc waitForTurn() {
  myTurn$(here.id).readFE();
}


proc passTurn() {
  myTurn$((here.id + 1) % numLocales).writeEF(true);
  if (here.id == 0) then {
    myTurn$(here.id).readFE();
    myTurn$(here.id).writeEF(true);
  }
}


proc writelnFragArray(startStr, X, endStr) {
  if (here.id == 0) {
    write(startStr);
  }
  waitForTurn();
  if (here.id != 0 && X.size != 0) {
    write(" ");
  }
  write(X);
  passTurn();
  if (here.id == 0) {
    writeln(endStr);
  }
}


proc BlockPartition(D, myBlock, numBlocks) {
  const lo = D.dim(0).low;
  const hi = D.dim(0).high;
  const numelems = hi - lo + 1;
  const nbAsIndexType = numBlocks:index(D);
  const myBlockAsIndexType:index(D) = myBlock;
  const blo = lo + (myBlockAsIndexType*numelems) / nbAsIndexType;
  const bhi = lo + (((myBlockAsIndexType+1)*numelems) / nbAsIndexType) - 1;
  return {blo..bhi};
}
