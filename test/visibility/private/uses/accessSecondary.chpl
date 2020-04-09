use User; // from definesSecondary.chpl
use TypeDefiner; // same

// Verifies that you can make use of secondary methods obtained via
// another module's private use.
var foo: borrowed Foo = new Foo(5);
foo.other();
