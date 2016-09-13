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
proc wrong.Key(a) {
  return abs(a);
}

// Invalid key args -- should have 1 arg
record keyargs{ }
proc keyargs.key(a, b) {
  return a - b;
}

// Invalid compareargs -- should have 2 args
record compareargs { }
proc compareargs.compare(a) {
  return a;
}
