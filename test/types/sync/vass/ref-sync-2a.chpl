// Passing a sync variable to a non-sync 'ref' formal, same underlying type.

proc fun(ref x: int,  y:int)  {}

var a: sync int;
var b: int;
fun(a, b); // error: value from coercion passed to ref formal
