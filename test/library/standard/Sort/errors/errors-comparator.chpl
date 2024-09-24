/*
 *  Tests compile errors upon passing a broken comparator
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

/* Broken comparators */

// Empty comparator
record empty { }

// Wrong function
record wrong { }
wrong implements keyComparator;
proc wrong.Key(a) {
  return abs(a);
}

// Invalid key args -- should have 1 arg
record keyargs { }
keyargs implements keyComparator;
proc keyargs.key(a, b) {
  return a - b;
}

// Invalid compareargs -- should have 2 args
record compareargs { }
compareargs implements relativeComparator;
proc compareargs.compare(a) {
  return a;
}

// Bad keyPart args -- need 2 args
record keyPartArgs { }
keyPartArgs implements keyPartComparator;
proc keyPartArgs.keyPart(a) {
  return a;
}

// Both key and compare
record keyAndCompare { }
proc keyAndCompare.key(a) {
  return a;
}
proc keyAndCompare.compare(a, b) {
  return 0;
}

// Both key and keyPart
record keyAndKeyPart { }
proc keyAndKeyPart.key(a) {
  return a;
}
proc keyAndKeyPart.keyPart(a, i) {
  return (0, 0);
}
