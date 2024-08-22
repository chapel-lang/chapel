// Tests the impact of parSafe on the type, especially in scenarios where
// two variables differ in their parSafe settings
use Set;

var x: set(int);
x.add(1); x.add(2);
writeln("x: ", x.type:string);

var a: set(?) = x;
writeln("a: ", a.type: string);
var b: set(parSafe=false, ?) = x;
writeln("b: ", b.type: string);
var c: set(parSafe=true, ?) = x;
writeln("c: ", c.type: string);

var y: set(parSafe=true, int);
y.add(1); y.add(2);
writeln("y: ", y.type: string);

var d: set(?) = y;
writeln("d: ", d.type: string);
var e: set(parSafe=false, ?) = y;
writeln("e: ", e.type: string);
var f: set(parSafe=true, ?) = y;
writeln("f: ", f.type: string);

var g = y;
writeln("g: ", g.type: string);
var h: set(int, false) = y;
writeln("h: ", h.type: string);
var i: set(int) = y;
writeln("i: ", i.type: string);
