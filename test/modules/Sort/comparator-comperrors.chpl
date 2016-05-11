/*
 *  Confirms that passing a broken comparators will result in compile error
 *
*/

use Sort;

config type comparator;

proc main() {

  // Array to sort
  var Arr = [-1,-4, 2, 3];

  var comp = new comparator();

  // Test fails if this compiles
  BubbleSort(Arr, comparator=comp);

}

/* Broken comparators */

// Empty comparator
record empty { }

// Wrong function
record wrong { }
proc wrong.Key(a) {
  return abs(a);
}

// Invalid key args
record keyargs{ }
proc keyargs.key(a, b) {
  return a - b;
}

// Invalid compareargs
record compareargs { }
proc compareargs.key(a, b) {
  return a - b;
}

/* Unhandled errors:

// Wrong return type -- hangs
record keyreturn { }
proc keyreturn.key(a) {
  return 42;
}
*/

/* Working comparators */

record working{ }
proc working.key(a) { return abs(a); }
