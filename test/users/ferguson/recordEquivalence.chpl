// recordEquivalence.chpl
//
// submitted by Michael Ferguson 2011/04/19
//
// Given what it says in the spec about Record Arguments, I expect the 
// following program to compile, run, and print out 300. 
//

record A {
  var x: int;
  var y: int;
}

record B {
  var x: int;
  var y: int;
}

proc sum_x_y(point:A) {
  return point.x + point.y;
}

var mypoint:B;
mypoint.x = 100;
mypoint.y = 200;

// According to the spec, the argument should be implicitly cast B->A here.
var sum = sum_x_y(mypoint);

writeln(sum);
