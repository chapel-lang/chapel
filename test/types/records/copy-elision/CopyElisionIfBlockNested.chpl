record r {
  proc init() { writeln('default init'); }
  proc init=(other: r) { writeln('init='); }
  proc deinit() { writeln('deinit'); }
}

operator r.=(ref lhs: r, rhs: r) { writeln('assign'); }

proc consumeElement(in elem) {}

proc test1() {
  writeln('T1');
  var doBlock = false;
  if doBlock {
    var x = new r();
    if doBlock {
      var a = x;
      consumeElement(a);
    } else {
      var b = x;
      consumeElement(b);
    }
  } else {
    var y = new r();
    if doBlock {
      var a = y;
      consumeElement(a);
    } else {
      var b = y;
      consumeElement(b);
    }
  }
}
test1();

