// Domain with empty range for first dimension
var D = {1..0, 1..2};

// Two slices for the empty range, which should both 
// result in an empty range.  The first one does not.
writeln(D[..1, ..]);
writeln(D[1.., ..]);

writeln(D[..2, ..]);
writeln(D[2.., ..]);


