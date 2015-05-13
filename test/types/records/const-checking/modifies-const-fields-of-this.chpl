// This test locks in the following behavior.
// A function that modifies const fields of 'this'
// can be invoked directly from a constructor,
// and cannot be invoked otherwise.
// Mostly copied from distributions/diten/Cyclic.chpl

record CyclicRecord {
  const targetLocDom: domain(1);
  const targetLocs: [targetLocDom] locale;
  const locDist: [targetLocDom] object;

  proc CyclicRecord() {
    helpConstruct();
  }

  proc CyclicRecord(arg:int) {
    var other = new CyclicRecord();
    other.helpConstruct();           // error
  }

  proc helpConstruct() {
    for locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new object(); // OK
  }
}

record CyclicClass {
  const targetLocDom: domain(1);
  const targetLocs: [targetLocDom] locale;
  const locDist: [targetLocDom] object;

  proc CyclicClass() {
    helpConstruct();
  }

  proc CyclicClass(arg:int) {
    var other = new CyclicClass();
    other.helpConstruct();           // error
  }

  proc helpConstruct() {
    for locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new object(); // OK
  }
}

proc main {
  var cr = new CyclicRecord();
  cr.helpConstruct();     // error
  new CyclicRecord(22);

  var cc = new CyclicClass();
  cc.helpConstruct();     // error
  new CyclicClass(22);
}
