use Sort;

// deprecation, missing keyPartComparator
record R1 {
  proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;
  proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt);
}

// this is an error, keyPart is wrong
record R2: keyPartComparator {
  proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;
  proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt);
}

// this is correct
record R3: keyPartComparator {
  proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;
  proc keyPart(elt, i) do
    return (if i > 0 then keyPartStatus.pre else keyPartStatus.returned, elt);
}


config type comparator;

use Random;
var arr: [1..1000] int;

fillRandom(arr);
writeln("isSorted before ", isSorted(arr, comparator=new comparator()));
// this line won't get the deprecation warning (already resolved in isSorted)
sort(arr, comparator=new comparator());
writeln("isSorted after ", isSorted(arr, comparator=new comparator()));
