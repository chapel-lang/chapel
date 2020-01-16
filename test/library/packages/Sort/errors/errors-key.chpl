/*
 *  Tests compile errors upon passing a non-string/numeric comparator.key(a)
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

/* Broken keys */

// Return class 'foobar'
record rec { }
class foobar { };

proc rec.key(a) {
  return new foobar();
}


/* TODO -- unhandled errors

// Key returns void - Chapel can't check if a function has a void return type
record voidCmp { }

proc voidCmp.key(a) { }
*/
