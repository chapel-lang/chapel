config const n = 5;

var D: domain(2) = [1..n, 1..n];

//
// I'm writing this test making this illegal.  But arguably, it
// could also be used to change D into the domain filled with
// degenerate ranges?  [0..-1, 0..-1];
//
D.clear();
