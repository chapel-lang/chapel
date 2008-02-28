config const debugSync = false;

var myTurn$: sync int = 0;
var barrierCount$: sync int = 0;
var barrierCount2$: sync int = 0;
var barrierDone$: sync bool = false;

def waitForTurn() {
  if (debugSync) then _debugWriteln(localeID(), " waiting for turn");
  while (myTurn$.readXX() != localeID()) { }
  if (debugSync) then _debugWriteln(localeID(), " got for turn");
  myTurn$;
}


def passTurn() {
  if (debugSync) then _debugWriteln(localeID(), " passing turn");
  myTurn$ = (localeID() + 1) % numLocales;
  if (debugSync) then _debugWriteln(localeID(), " passed turn");
  if (localeID() == 0) then while (myTurn$.readXX() != localeID()) { }
}

def barrier() {
  // BLC: I'm not confident this is correct
  if (debugSync) then _debugWriteln(localeID(), " entering barrier");
  barrierCount$ += 1;
  if (debugSync) then _debugWriteln(localeID(), " incremented barrier");
  if (localeID() == 0) {
    if (debugSync) then _debugWriteln(localeID(), " waiting on 0");
    while (barrierCount$.readXX() != numLocales) { }
    if (debugSync) then _debugWriteln(localeID(), " resetting vars");
    barrierCount2$.writeFF(0);
    barrierCount$.writeFF(0);
    barrierDone$.writeFF(true);
  }
  if (debugSync) then _debugWriteln(localeID(), " waiting for things to be done");
  while (barrierDone$.readXX() != true) { }
  barrierCount2$ -= 1;
  if (localeID() == 0) {
    while (barrierCount2$.readXX() != -numLocales) { }
    barrierCount2$.writeFF(0);
    barrierDone$.writeFF(false);
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
