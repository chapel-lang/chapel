// A variable of type sync T should be like a variable of type T
// in all ways with respect to its value. So it should be coercible
// to a bool within an if.

var i$: sync int = 5;
if i$ then writeln("yes");
