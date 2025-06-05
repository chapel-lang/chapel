//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Sort;

record keyPartTupleInt: keyPartComparator {
/* START_EXAMPLE_TUPLE_INT */
proc keyPart(elt: 2*int, i: int) {
  if i > 1 then
    return (keyPartStatus.pre, 0); // second value is not used

  return (keyPartStatus.returned, elt(i));
}
/* STOP_EXAMPLE_TUPLE_INT */
}


var ArrayTuples = [(2, 3), (1, 4), (2, 5), (3, 6)];
sort(ArrayTuples, comparator = new keyPartTupleInt());
writeln(ArrayTuples);

record keyPartString: keyPartComparator {
/* START_EXAMPLE_STRING */
proc keyPart(x: string, i: int): (keyPartStatus, uint(8)) {
  var len = x.numBytes;
  var section = if i < len then keyPartStatus.returned  else keyPartStatus.pre;
  var part =    if i < len then x.byte(i)               else 0:uint(8);
  return (section, part);
}
/* STOP_EXAMPLE_STRING */
}

var ArrayStrings = ["jkl", "abc", "def", "ghi", "jklmno"];
sort(ArrayStrings, comparator = new keyPartString());
writeln(ArrayStrings);
