use Reflection;
record r {}
var x = [1,2];
var y = [new r()];
if canResolve('<=', x, y) {
  var ret = x <= y;
  writeln(ret); // runs and prints true, since x < y
}
