// Paired with noDefaultValue.chpl to demonstrate that the code would work with
// minor changes (using a nilable type instead of a non-nilable type)
use SortedSet;

class Foo {
  var x: int;

  operator <(x: Foo?, y: Foo?) {
    return (x!).x < (y!).x;
  }

  operator <(x: Foo, y: Foo) {
    return x.x < y.x;
  }
}

var s1 = new sortedSet(shared Foo?, false, new DefaultComparator());
s1.add(new shared Foo(11));
s1.add(new shared Foo(5));
s1.add(new shared Foo(16));

var firstFound: bool;
var firstResult: shared Foo?;
(firstFound, firstResult) = s1.kth(1);
writeln(firstFound);  // If no compiler error, should be `true`
writeln(firstResult); // If no compiler error, should be `{x = 5}`
