var myTurn$: [LocaleSpace] sync bool;
myTurn$(0) = true;


def waitForTurn() {
  myTurn$(here.id);
}


def passTurn() {
  myTurn$((here.id + 1) % numLocales) = true;
  if (here.id == 0) then {
    myTurn$(here.id);
    myTurn$(here.id) = true;
  }
}


def writelnFragArray(startStr, X, endStr) {
  if (here.id == 0) {
    write(startStr);
  }
  waitForTurn();
  if (here.id != 0 && X.numElements != 0) {
    write(" ");
  }
  write(X);
  passTurn();
  if (here.id == 0) {
    writeln(endStr);
  }
}


def BlockPartition(D, myBlock, numBlocks) {
  const lo = D.dim(1)._low;
  const hi = D.dim(1)._high;
  const numelems = hi - lo + 1;
  const nbAsIndexType = numBlocks:index(D);
  const myBlockAsIndexType:index(D) = myBlock;
  const blo = lo + (myBlockAsIndexType*numelems) / nbAsIndexType;
  const bhi = lo + (((myBlockAsIndexType+1)*numelems) / nbAsIndexType) - 1;
  return [blo..bhi];
}
