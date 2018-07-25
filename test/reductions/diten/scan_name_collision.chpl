config var n = 10;

var        r          = new unmanaged R();
var        s          = new unmanaged R();
var        longerName = new unmanaged R();

writeln(+ scan [1..n] r.next());
writeln(+ scan [1..n] s.next());
writeln(+ scan [1..n] longerName.next());

delete longerName;
delete s;
delete r;

class R {
  var count = 0;

  proc next() {
    count += 1;

    return count;
  }
}
