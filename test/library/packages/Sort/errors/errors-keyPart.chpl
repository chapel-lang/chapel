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

proc str.keyPart(a, i) {
  return 'foobar';
}


record tStrStr { }

proc tStrStr.keyPart(a, b) {
  return ("a", "b");
}

record tUintInt { }

proc tUintInt.keyPart(a, b) {
  return (0:uint, 0);
}

record tIntReal { }

proc tIntReal.keyPart(a, b) {
  return (0, 0.0);
}
