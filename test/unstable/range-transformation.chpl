//Range

var r1 = 1..5;
var r2 = r1.translate(2);
var r3 = r1.interior(0);


// Domain
var d1 = {1..5, 1..5};
var d2 = d1.translate(2, 3); // Testing Overload 2
var d3 = d1.translate( (2,3) ); // Overload 3
var d4 = d1.translate(2); // Overload 4

d2 = d1.interior(0, 0);  // Testing Overload 2
d3 = d1.interior( (0, 0)); // Overload 3
d4 = d1.interior(0); // Overload 4
