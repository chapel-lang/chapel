//Range

var r1 = 1..5;
var r2 = r1.translate(2);



// Domain
var d1 = {1..5, 1..5};
var d2 = d1.translate(2, 3); // Testing Overload 2
d2 = d1.translate( (2,3) ); // Overload 3
var d3 = d1.translate(2); // Overload 4
