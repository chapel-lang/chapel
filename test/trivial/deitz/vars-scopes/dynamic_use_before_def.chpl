/*
  The current expectation for Chapel is that the function foo() will be
  executed before x is initialized and hence the value that is printed
  is undefined.

  Historically this future has failed due to an implementation detail
  among the resolution passes.  In early 2017, updates to resolution that
  are intended to enhance record initialization caused the initial version
  of this future to start to pass.

  An update to the future will cause it to fail again.  Thought is required
  to identify a more principled analysis that can detect that this is an
  ill-defined program.
*/



foo();

var x: int;

proc foo() {
  writeln(x);
}
