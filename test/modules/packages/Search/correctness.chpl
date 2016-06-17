/*
 *  Check correctness of search functions
 */

use Search;

proc main() {

  var result: (bool, int);

  // Sorted arrays
  const    Arr = [-4, -1, 2, 3],
        ArrAbs = [-1, 2, 3, -4],
        ArrRev = [ 3, 2, -1, -4],
     ArrAbsRev = [ -4, 3, 2, -1],
        StrArr = ['Brad', 'anthony', 'ben', 'david'];

  // Comparators
  const key = new keycomparator(),
        compare = new compcomparator(),
        tuplekey = new tuplecomparator();


  /* Test data types */

  // Integers
  result = search(Arr, 2);
  checkSearch(result, (true, 3), Arr, 'search');

  // Strings
  result = search(StrArr, 'ben');
  checkSearch(result, (true, 3), StrArr, 'search');

  // Integers sorted as tuples
  result = search(Arr, 2, comparator=tuplekey);
  checkSearch(result, (true, 3), Arr, 'search', 'tuplekey');

  /* Reverse */

  // Default reversecomparator
  result = search(ArrRev, 2, comparator=reverseComparator);
  checkSearch(result, (true, 2), ArrRev, 'search');

  /* Comparator basics */
  result = search(ArrAbs, 2, comparator=key);
  checkSearch(result, (true, 2), ArrAbs, 'search', 'key');

  result = search(ArrAbs, 2, comparator=compare);
  checkSearch(result, (true, 2), ArrAbs, 'search', 'compare');

  /* Not Found */

  result = search(Arr, 5);
  checkSearch(result, (false, Arr.domain.high+1), Arr, 'search');

  result = search(Arr, -5);
  checkSearch(result, (false, Arr.domain.low), Arr, 'search');

  result = search(Arr, 0);
  checkSearch(result, (false, 3), Arr, 'search');

  /* Test Searches */

  // linearSearch
  result = linearSearch(Arr, 2);
  checkSearch(result, (true, 3), Arr, 'linearSearch');

  // binarySearch
  result = binarySearch(Arr, 2);
  checkSearch(result, (true, 3), Arr, 'binarySearch');

}


/* Checks array and resets values -- any output results in failure */
proc checkSearch(result, expected, array, searchProc:string, comparator:string='none') {
  if result != expected {
    writeln(searchProc, ' with comparator=', comparator, ' failed');
    writeln('For array:');
    writeln(array);
    writeln('Incorrect result:');
    writeln(result);
    writeln('Expected result:');
    writeln(expected);
  }
}


/* Defines manipulation of value that should be used for comparison */
record keycomparator { }
proc keycomparator.key(a) { return abs(a); }


/* Defines compare behavior, return 1, 0, or -1 */
record compcomparator { }
proc compcomparator.compare(a, b) {
  return abs(a) - abs(b);
}

/* Key method can return a non-numerical/string type, such as tuple */
record tuplecomparator { }
proc tuplecomparator.key(a) { return (a, a); }
