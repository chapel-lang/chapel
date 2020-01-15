/*
  Jan 12, 2016: Michael Noakes

  Our internal implementation of sparse arrays currently relies on
  the use of the setter param in a procedure with return by ref-intent.

  This simple test provides some confidence that this continues to
  work correctly while
     a) we stabilize strings
     b) we refine the definition of return-by-ref and the setter param
*/

var D                        = { 1 .. 10 };
var SD : sparse subdomain(D) = { 3 };

var A  : [SD] string;

A[3] = "hi";

for i in D do
  writeln(A[i]);
