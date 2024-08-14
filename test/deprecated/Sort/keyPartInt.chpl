use Sort, CTypes;

//
// Static usage, a user tries to call `keyPart` from DefaultComparator or
// ReverseComparator without `-suseKeyPartStatus`
//

var comparators = (new DefaultComparator(), new ReverseComparator());
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

record myCmp {
  // this is a simple comparator for integers
  // its identical to the Sort module for normal sorting
  proc keyPart(elt, i) {
    var section = if i > 0 then -1 else 0;
    return (section, elt);
  }
}
use Random;
var arr: [1..1000] int;
fillRandom(arr);

// there should be 6 deprecations here
sort(arr, comparator=new myCmp());
writeln("isSorted ", isSorted(arr, comparator=new myCmp()));

sort(arr, comparator=new ReverseComparator(new myCmp()));
writeln("isSorted ", isSorted(arr, comparator=new ReverseComparator(new myCmp())));

var temp = sorted(arr, comparator=new MyCmp());
writeln("isSorted ", isSorted(temp, comparator=new myCmp()));
