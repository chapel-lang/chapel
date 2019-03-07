class C { 
  var x: domain;
}
proc writeDom(y: domain) {
  writeln(y);
}
var c = new unmanaged C({1..2});
writeln(c);
writeDom(c.x);
delete c;
