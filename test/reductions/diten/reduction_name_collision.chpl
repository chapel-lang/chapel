config var n = 10;
var r = new R();
var s = new R();
var longerName = new R();
writeln(+ reduce [1..n] r.next());
writeln(+ reduce [1..n] s.next());
writeln(+ reduce [1..n] longerName.next());

class R {
  var count: sync int = 0;
  proc next() {
    var c = count + 1;
    count = c;
    return c;
  }
}
