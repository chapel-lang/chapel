use Reflection;
record r {}
var x = [1];
var y = [new r()];
if canResolve('<=', x, y) {
  var ret = x <= y;
  writeln(ret); // doesn't run; can't resolve `<=` between arrays or between 1 and `new r()`.
}
