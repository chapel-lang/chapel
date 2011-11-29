//
// These capture our current precedence rules for reductions
// as compared to math operators.  Our rationale for making
// them bind more tightly is due to the first triple of
// reductions and the belief that accidentally generating an
// array value (as in the first statement of the second
// group) is the kind of thing that you'll quickly figure out;
// this is combined with a sense that reductions are unary
// operator-like, even though they take the reduction operation
// as one of their two operands.
//

var A: [1..3] int = (1, 5, 9);
var B: [1..3] int = (1, 2, 3);

writeln(+ reduce A + max reduce B);
writeln(+ reduce (A + max reduce B));
writeln((+ reduce A) + (max reduce B));
writeln();

writeln(+ reduce A*A);
writeln(+ reduce (A*A));
writeln((+ reduce A) * A);
writeln();

writeln(+ reduce [a in A] a*a);
writeln();
