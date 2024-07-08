use User; // from definesSecondary.chpl
use TypeDefiner; // same

// Verifies that you cannot make use of secondary methods obtained via
// another module's private use.
var myOwnedFoo = new Foo(5);
var foo: borrowed Foo = myOwnedFoo;
foo.other();
