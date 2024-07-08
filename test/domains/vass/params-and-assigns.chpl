
// The stride being a param for these domains is a recently-added nicety.
// It is relied on in the check 'if Dom.stride != 1' in Sort.quickSort().

var d1: domain(1, color, strideKind.one);
var d2: domain(1, int, strideKind.negOne);
compilerWarning(d1.stride:string, d2.stride:string);

// Assignments between domains and ranges of the same enum idxType are OK.

enum color { red };

var r1: range(color, boundKind.both, strideKind.positive);
var r2: range(color, boundKind.both, strideKind.any);
r2 = r1;

var d3: domain(1, color, strideKind.negOne);
var d4: domain(1, color, strideKind.negative);
d4 = d3;

var d5: domain(2, color, strideKind.negative);
var d6: domain(2, color, strideKind.any);
d6 = d5;

writeln((r1,r2,d1,d2,d3,d4,d5,d6));
