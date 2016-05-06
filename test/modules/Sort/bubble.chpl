use Reflection;

private inline proc chpl_sort_cmp(in a, in b, param reverse=false, param eq=false, functions:?t=new defaultFunctions()) {
  if canResolveMethod(functions, "key", a) && canResolveMethod(functions, "key", b) {
    a = functions.key(a);
    b = functions.key(b);
  } else {
    compilerError("'functions' record passed to sort does not contain a valid 'key' method");
  }
  if eq {
    if reverse then return a >= b;
    else return a <= b;
  } else {
    if reverse then return a > b;
    else return a < b;
  }
}

/**
  * Base compare function for all sorts
  * Returns -1 if a > b, 0 if a == b, and -1 if a < b
  *
  **/
private inline proc chpl_compare(a, b, param reverse=false, comparator:?t=new defaultComparator()): int {
  // Use comparator.compare(a, b) if is defined by user
  if canResolveMethod(comparator, "compare", a, b) then
    if reverse then return -comparator.compare(a, b)
               else return  comparator.compare(a, b);

  // TODO -- It may be faster to call key() on all data once, and mirror swaps
  // Compare results of comparator.key(a) if is defined by user
  if canResolveMethod(comparator, "key", a) && canResolveMethod(comparator, "key", b) then
    return chpl_compare(comparator.key(a), comparator.key(b), reverse);

  // TODO -- No idea if this is optimal order of checks
  if reverse then
    if a < b then return 1
      else if a > b return -1;
  else
    if a < b then return -1
      else if a > b return 1;

  return 0;
}

pragma "no doc"
record defaultComparator{ }

/*
   Sort the 1D array `Data` in-place using a sequential bubble sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: `bool`
   :arg reverse: Sort in reverse numerical order
   :type reverse: `bool`
   :arg functions: Record with method: 'functions.key(a)'

 */
proc BubbleSort(Data: [?Dom] ?eltType, doublecheck=false, param reverse=false, comparator:?t=new defaultComparator()) where Dom.rank == 1 {
  // TODO -- Dom.rank doesn't have to be 1 if a comparator record is passed
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  var swapped = true;

  while (swapped) {
    swapped = false;
    for i in lo..hi-1 {
      if chpl_compare(Data(i), Data(i+1), reverse, comparator) >= 0 {
        Data(i) <=> Data(i+1);
        swapped = true;
      }
    }
  }

  if (doublecheck) then VerifySort(Data, "BubbleSort", reverse, comparator);
}

inline proc VerifySort(Data: [?Dom] ?eltType, str: string, param reverse=false, comparator:?t=new defaultComparator()) {
  for i in Dom.low..Dom.high-1 do
    if chpl_compare(Data(i+1), Data(i), reverse, comparator) < 0 then
      halt(str, " did not sort properly (", i, "): ", Data);
}
