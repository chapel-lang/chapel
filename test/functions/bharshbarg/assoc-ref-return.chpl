
// array of arrays, incorrect behavior
writeln("Testing associative array of arrays...");

var dom = {1,3,5};
var data : [dom] [1..5] int;
for d in data do d = 3;

// OK, '1' exists in `dom`
var x = data[1][1];
writeln(x);

// Should see an out-of-bounds error, but the 'ref' case in
// DefaultAssociative is triggered and an index/element added to the array.
var y = data[50][1];
writeln(y);

//
// Shouldn't reach this point due to the out-of-bounds above.
//
// Create a second array defined over `dom` which will disable the add-assign
// feature and result in a halt, making it extra-clear that a write is
// occurring.
//
var other : [dom] [1..5] int;
var z = data[100][1];
writeln(z);
