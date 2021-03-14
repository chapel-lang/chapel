config var n = 10;

var        r          = new unmanaged R();
var        s          = new unmanaged R();
var        longerName = new unmanaged R();

writeln(+ scan for 1..n do r.next());
writeln(+ scan for 1..n do s.next());
writeln(+ scan for 1..n do longerName.next());

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
