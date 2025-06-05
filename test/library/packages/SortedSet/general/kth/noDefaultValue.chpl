// Paired with yesDefaultValue.chpl to demonstrate that the code would fail with
// minor changes (using a non-nilable type instead of a nilable type).
//
// This test locks in that we generate the compiler error we intend from `kth`
// (prior to a fix, we were checking the wrong thing and so encountering an
// error later in the body of `kth`)
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

var s1 = new sortedSet(shared Foo, false, new defaultComparator());
s1.add(new shared Foo(11));
s1.add(new shared Foo(5));
s1.add(new shared Foo(16));

var firstFound: bool;
// To avoid default initialization in the test code, give it a bogus initial
// value
var firstResult: shared Foo = new shared Foo(-1);
(firstFound, firstResult) = s1.kth(1); // Expect a compiler error from kth
writeln(firstFound);  // If no compiler error, should be `true`
writeln(firstResult); // If no compiler error, should be `{x = 5}`
