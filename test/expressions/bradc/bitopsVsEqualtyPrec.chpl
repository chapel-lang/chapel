//
// This test shows the relative precedence between bitwise ops
// and ==.  Our gut is to make the first statement behave like
// the second -- this seems most natural to us, and according
// to the web to many others as well.  However, we perhaps  have
// a stronger desire to conform to C, C++, C#, Java, and Perl
// on this topic, and they all make the equality bind more
// tightly.  The main rationale for this is to allow what we
// think of here as a bitwise operator to be used as a 
// non-short-circuiting logical operator.  gcc -Wall offers
// warnings for cases like the first below, which we may want
// to adopt as well.
//

var x = 3;
var y = 5;

writeln(x | y == 7);
writeln((x | y) == 7);
writeln(x | (y == 7));
