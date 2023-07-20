//Range
var r1 = 1..5;
var r2 = r1.translate(2);
r2 = r1.interior(0);
r2 = r1.exterior(0);
r2 = r1.expand(0);
var r3 = (1..5 by 2).offset(2);
var r4 = (1..10).indexOrder(4);
var r7 = (1..10).orderToIndex(3);

// Domain
var d1 = {1..5, 1..5};
var d2 = d1.translate(2, 3); // Testing Overload 2
var d3 = d1.translate( (2,3) ); // Overload 3
var d4 = d1.translate(2); // Overload 4

d2 = d1.interior(0, 0);  // Testing Overload 2
d3 = d1.interior( (0, 0)); // Overload 3
d4 = d1.interior(0); // Overload 4

d2 = d1.exterior(0, 0);  // Testing Overload 2
d3 = d1.exterior( (0, 0)); // Overload 3
d4 = d1.exterior(0); // Overload 4

d2 = d1.expand(0, 0);  // Testing Overload 2
d3 = d1.expand( (0, 0)); // Overload 3
d4 = d1.expand(0); // Overload 4

var d5 = d1.orderToIndex(3);
