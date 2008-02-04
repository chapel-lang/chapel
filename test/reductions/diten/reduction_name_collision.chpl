config var n = 10;
var r = new R();
var s = new R();
var longerName = new R();
writeln(+ reduce [1..n] r.next());
writeln(+ reduce [1..n] s.next());
writeln(+ reduce [1..n] longerName.next());

class R {
  var count = 0;
  def next() {
    count += 1;
    return count;
  }
}
