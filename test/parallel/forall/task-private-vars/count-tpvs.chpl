// Ensure no extra TPVs get created.

const localTasks = here.maxTaskPar;
const totalTasks = + reduce Locales.maxTaskPar;

var a: atomic int;
record R { proc init() { a.add(1); } }

writeln("range");  /////////////////////////////////
a.write(0);

forall i in 1..localTasks with (var r = new R()) { }
assert(a.read() == localTasks);

writeln("block");  /////////////////////////////////
a.write(0);

use BlockDist;
var A = newBlockArr({1..10000}, int);

forall a in A with (var r = new R()) { }
assert(a.read() == totalTasks);

writeln("done");   /////////////////////////////////
