// Base case: use of enum allows unqualified access to its components
enum foo {a, b, c};

use foo;

var first = a;
var second = b;
var third: foo = c;
writeln(first);
writeln(second);
writeln(third);
