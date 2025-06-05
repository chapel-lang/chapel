/*
 *  Tests compile errors upon passing a non-string/numeric comparator.compare(a, b)
 */

use Sort;

config type comparator;

proc main() {

  // Array to sort
  var Arr = [-1,-4, 2, 3];

  var comp = new comparator();

  // Test fails if this compiles
  sort(Arr, comparator=comp);

}


/* Broken keyPart */

record str { }
str implements keyPartComparator;

proc str.keyPart(a, i) {
  return 'foobar';
}


record tStrStrDep { }

proc tStrStrDep.keyPart(a, b) {
  return ("a", "b");
}

record tStrStr { }
tStrStr implements keyPartComparator;

proc tStrStr.keyPart(a, b) {
  return ("a", "b");
}

// deprecation warning and error about uint instead of int
record tUintIntDep { }

proc tUintIntDep.keyPart(a, b) {
  return (0:uint, 0);
}

record tUintInt { }
tUintInt implements keyPartComparator;

proc tUintInt.keyPart(a, b) {
  return (0:uint, 0);
}

record tIntReal { }
tIntReal implements keyPartComparator;

proc tIntReal.keyPart(a, b) {
  return (keyPartStatus.returned, 0.0);
}
