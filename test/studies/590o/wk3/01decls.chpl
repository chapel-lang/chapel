/* Basic declarations using:
 *   - type + initializer
 *   - type only
 *   - initializer only
 */

var i1: int = 1;
var i2: int;
var i3 = 3;

writeln("i1 is: ", i1);
writeln("i2 is: ", i2);
writeln("i3 is: ", i3);
writeln();


/* Multiple variables of the same type and/or initializer: */

var r1, r2: real = 1.2;
var r3, r4: real;
var r5, r6 = 3.4;

writeln("r1 is: ", r1);
writeln("r2 is: ", r2);
writeln("r3 is: ", r3);
writeln("r4 is: ", r4);
writeln("r5 is: ", r5);
writeln("r6 is: ", r6);
writeln();


/* Multiple variables of varying types/initializers: */

var flag1, flag2 = true,
    pi = 3.1415,
    numRows, numCols: int,
    name1 = "Brad", name2 = "Steve";

writeln("flag1 is: ", flag1);
writeln("flag2 is: ", flag2);
writeln("pi is: ", pi);
writeln("numRows is: ", numRows);
writeln("numCols is: ", numCols);
writeln("name1 is: ", name1);
writeln("name2 is: ", name2);
writeln();


/* Basic array declarations (using anonymous domains): */

var A1: [1..9, 1..9] bool;
const A2: [{1..9, 1..9}] real = [(i,j) in {1..9, 1..9}] i + j/10.0;

writeln("A1 is:\n", A1);
writeln("A2 is:\n", A2);
writeln();
