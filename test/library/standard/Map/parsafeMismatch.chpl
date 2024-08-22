// Tests the impact of parSafe on the type, especially in scenarios where
// two variables differ in their parSafe settings
use Map;

var x: map(int, int);
x.add(1, 1); x.add(2, 2);
writeln("x: ", x.type:string);

var a: map(?) = x;
writeln("a: ", a.type: string);
var b: map(parSafe=false, ?) = x;
writeln("b: ", b.type: string);
var c: map(parSafe=true, ?) = x;
writeln("c: ", c.type: string);

var y: map(parSafe=true, int, int);
y.add(1, 1); y.add(2, 2);
writeln("y: ", y.type: string);

var d: map(?) = y;
writeln("d: ", d.type: string);
var e: map(parSafe=false, ?) = y;
writeln("e: ", e.type: string);
var f: map(parSafe=true, ?) = y;
writeln("f: ", f.type: string);

var g = y;
writeln("g: ", g.type: string);
var h: map(int, int, false) = y;
writeln("h: ", h.type: string);
var i: map(int, int) = y;
writeln("i: ", i.type: string);
