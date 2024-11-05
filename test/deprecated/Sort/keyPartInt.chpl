use Sort, CTypes;

//
// Static usage, a user tries to call `keyPart` from defaultComparator or
// reverseComparator without `-suseKeyPartStatus`
//

var comparators = (new defaultComparator(), new reverseComparator());
var titles = ("Default", "Reverse");

for param i in 0..#comparators.size {
  writeln("Comparing with ", titles[i], "Comparator");
  var d = comparators[i];
  // integral
  writeln(d.keyPart(1, 0));
  writeln(d.keyPart(2:uint, 0));
  // real(?)
  writeln(d.keyPart(2.0, 0));
  // imag(?)
  writeln(d.keyPart(2.0i, 0));
  // _tuple
  writeln(d.keyPart((4, 2), 0));
  writeln(d.keyPart((2.0, 8.0), 0));
  // string
  writeln(d.keyPart("foo", 0));
  // c_ptrConst(c_char)
  writeln(d.keyPart("bar".c_str(), 0));
}

//
// Dynamic usage deprecation warnings, for when a user has defined a custom comparator and uses it in `sort` or `isSorted`
//


// This is a simple comparator for integers
// It is identical to the Sort module for normal sorting
// To make sure we get all the deprecation warnings, we need to use a unique comparator type for each call

record myCmp1 { proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt); }
record myCmp2 { proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt); }
record myCmp3 { proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt); }
record myCmp4 { proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt); }
record myCmp5 { proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt); }
record myCmp6 { proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt); }
record myCmp7 { proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt); }
record myCmp8 { proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt); }
record myCmp9 { proc keyPart(elt, i) do return (if i > 0 then -1 else 0, elt); }

record myCmpResolveDeps: keyPartComparator {
  proc keyPart(elt, i) do
    return (if i > 0 then keyPartStatus.pre else keyPartStatus.returned, elt);
}

proc getComparator(param i: int) type {
  if Sort.useKeyPartStatus then return myCmpResolveDeps;
  else select i {
    when 1 do return myCmp1;
    when 2 do return myCmp2;
    when 3 do return myCmp3;
    when 4 do return myCmp4;
    when 5 do return myCmp5;
    when 6 do return myCmp6;
    when 7 do return myCmp7;
    when 8 do return myCmp8;
    when 9 do return myCmp9;
  }
  compilerError("");
}

use Random;
var arr: [1..1000] int;

// there should be 9 deprecations here
fillRandom(arr);
writeln("isSorted before ", isSorted(arr, comparator=new (getComparator(1))()));
sort(arr, comparator=new (getComparator(2))());
writeln("isSorted after ", isSorted(arr, comparator=new (getComparator(3))()));

fillRandom(arr);
writeln("isSorted before ", isSorted(arr, comparator=new reverseComparator(new (getComparator(4))())));
sort(arr, comparator=new reverseComparator(new (getComparator(5))()));
writeln("isSorted after ", isSorted(arr, comparator=new reverseComparator(new (getComparator(6))())));

fillRandom(arr);
writeln("isSorted before ", isSorted(arr, comparator=new (getComparator(7))()));
var temp = sorted(arr, comparator=new (getComparator(8))());
writeln("isSorted after ", isSorted(temp, comparator=new (getComparator(9))()));
