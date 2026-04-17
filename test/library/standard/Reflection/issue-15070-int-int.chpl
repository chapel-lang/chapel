use Reflection;
record r {}
var x = [1];
var y = [2];
if canResolve('<=', x, y) {
  var ret = x <= y;
  writeln(ret); // runs and prints true, since x < y
}
