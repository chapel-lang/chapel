config const doit = false;

use BlockDist;
var A: [{1..1} dmapped Block({1..1})] real;
if doit then A.displayRepresentation();
compilerError("     *** READ THIS -- IMPORTANT ***     When updating this .good, also update test/distributions/deitz/test_block_representation*.good and any other .good files with the 'printing _ddata class' warning");
