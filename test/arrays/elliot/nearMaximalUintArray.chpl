// Show that we can create and assign into arrays that are close to but not
// quite maximal. Note that iteration might not work, if that iteration will
// result in range overflow.

var foo: [1:uint(8)..255:uint(8)] int;
var bar: [0:uint(8)..254:uint(8)] int;

foo[42:uint(8)] = 13;
bar[42:uint(8)] = 13;

writeln(foo(42:uint(8)));
writeln(bar(42:uint(8)));
