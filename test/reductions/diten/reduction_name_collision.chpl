config var n          = 10;

var        r          = new unmanaged R();
var        s          = new unmanaged R();
var        longerName = new unmanaged R();

writeln(+ reduce [1..n] r.next());
writeln(+ reduce [1..n] s.next());
writeln(+ reduce [1..n] longerName.next());

delete longerName;
delete s;
delete r;

class R {
  var count: sync int = 0;

  proc next() {
    var c = count.readFE() + 1;

    count.writeEF(c);

    return c;
  }
}
