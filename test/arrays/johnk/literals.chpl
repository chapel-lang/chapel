var justOne = [1];

var ints = [ 1,2,3 ];
var reals = [ 10.5, 11.3, 3.1415, 6.0, 9.10 ];
var imags = [ 10.5i, 11.3i, 3.1415i, 6.0i, 9.10i ];
var strings = [ "array", "literal", "strings", "go!"];
var procs = [ double(1), double(2), double(3), double(5), double(7) ];

var tuples = [ (1,2), (3,4), (5,6) ];
var arrays = [ [1,2,3], [4,5,6], [7,8,9] ];
var arrays2 = [ [10,11,12], [13,14,15], [16,17,18] ];
var arrays3 = [ arrays, arrays2 ];

proc double( x ){
  return 2 * x;
}

writeln(ints);
writeln(strings);
writeln(tuples);
writeln( arrays3 );
writeln( ["array", "literal", "strings"] );

if (&& reduce (ints == [1,2,3])){
  writeln("match");
}
