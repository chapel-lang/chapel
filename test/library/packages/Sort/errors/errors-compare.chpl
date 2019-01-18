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


/* Broken keys */

// Returns str
record str { }

proc str.compare(a, b) {
  return 'foobar';
}


// Returns bool
record boolean { }

proc boolean.compare(a, b) {
  return true;
}


// Return record 'foobar'
record rec { }
record foobar { };

proc rec.compare(a, b) {
  return new foobar();
}


/* TODO -- unhandled errors

// Key returns void - Chapel can't check if a function has a void return type
record voidCmp { }

proc voidCmp.key(a) { }
*/
