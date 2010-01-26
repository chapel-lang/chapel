/* I find that left-to-right declaration syntaxes work better than
 * inside-out/right-to-left when using arrays of arrays and/or
 * multidimensional arrays.  Unfortunately, arrays of arrays don't
 * currently work in our implementation, but once they do, the
 * following declarations of skyline, triangular, and hierarchical
 * arrays would work.  How do these declarations appear in your
 * favorite language?
 */

// Skyline array:
const numBuildings = 10;
const buildingHeight: [1..numBuildings] int = (3, 7, 9, 3, 4, 11, 2, 8, 6, 2);
//var skyline: [i in 1..numBuildings] [1..buildingHeight(i)] real;

writeln("buildingHeight is: ", buildingHeight);
//writeln("skyline is: ", skyline);


// Triangular array:
const n = 9;
//const Tri: [1..i] real = [i in 1..n] [j in 1..i] i + j/10.0;

//writeln("Tri is: ", Tri);


// 1D of 2D arrays of varying sizes:
const numLevels = 10;
//var HierArray: [lvl in 1..numLevels] [1..2**lvl, 1..2**lvl] real;
