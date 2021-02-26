// This was the snippet in the bug report
// https://github.com/chapel-lang/chapel/issues/15543

var s = 'string indexing is "fun"';

// These should all be true
writeln(s[0] == s[..0]);    // used to be false
writeln(s[0] == s[0..0]);   // used to be false

// Some other checks
writeln(s[0..1] == s[..1]); // true
writeln(s[1] == s[1..1]);   // true
writeln(s[1] == s[1..1]);   // true

writeln(s[..5] == s[0..5]); // true
writeln(s[2..2] == s[2]);   // true

// Sanity check to confirm it's only strings
var A = [1,2,3,4];

writeln(A[0] == A[..0]);   // true
writeln(A[0] == A[0..0]);  // true


writeln(s[0] == s[..0:byteIndex]);              // used to be false
writeln(s[0] == s[0:byteIndex..0:byteIndex]);   // used to be false

