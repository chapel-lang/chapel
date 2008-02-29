config const debugSync = false;

var myTurn$: [LocalesDomain] sync bool;
myTurn$(0) = true;

def waitForTurn() {
  if (debugSync) then _debugWriteln(localeID(), " waiting for turn");
  const tmp = myTurn$(localeID()); // BUG: tmp should not be required
  if (debugSync) then _debugWriteln(localeID(), " got for turn");
}


def passTurn() {
  if (debugSync) then _debugWriteln(localeID(), " passing turn");
  myTurn$((localeID() + 1) % numLocales) = true;
  if (debugSync) then _debugWriteln(localeID(), " passed turn");
  if (localeID() == 0) then {
    if (debugSync) then _debugWriteln(localeID(), " waiting for turn to come back around");
    const tmp = myTurn$(localeID()); // BUG: tmp should not be required
    if (debugSync) then _debugWriteln(localeID(), " resetting self");
    myTurn$(localeID()) = true;
    if (debugSync) then _debugWriteln(localeID(), " exiting passTurn");
  }
}

def writelnFragArray(startStr, X, endStr) {
  if (localeID() == 0) {
    write(startStr);
  }
  waitForTurn();
  if (localeID() != 0 && X.numElements != 0) {
    write(" ");
  }
  write(X);
  passTurn();
  if (localeID() == 0) {
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
