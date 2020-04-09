
class CC { var x: int; }

var u1y = new unmanaged CC(11)?;
var u2n = new unmanaged CC(20);

proc main {
  var oo = new owned(u1y);
  var ss = new shared(u2n);
  writeln(oo!.x);
  writeln(ss.x);
}
