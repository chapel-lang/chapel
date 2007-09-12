config var n = 10;
var r = R();
var s = R();
var longerName = R();
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
