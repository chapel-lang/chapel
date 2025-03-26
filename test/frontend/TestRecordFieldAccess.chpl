use Print;

record r {
  var x: int;
  proc method() { return x; }
}

// NOTE: Don't use 'new' as the new wrapper triggers a huge call-graph...
// var r1 = new r();
// 
var r1: r;

// TODO: Bugs when changing to 'ref'.
var x1 = r1.x;
var x2 = r1.method();

println(x1);
println(x2);

/* TODO: Followup...
r.x = 8;

println(x1);
println(x2);
*/
