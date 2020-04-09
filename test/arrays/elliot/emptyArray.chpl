// ensure that we can create empty arrays and init them without issue (make
// sure array init doesn't have any overflow issues, particularly for units.)
var foo: [1:uint(8)..0:uint(8)] int;
var bar: [1:int(8)..0:int(8)] int;

writeln(foo);
writeln(bar);
