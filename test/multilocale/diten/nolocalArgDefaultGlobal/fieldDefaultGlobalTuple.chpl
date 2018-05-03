var t = (1, 2);

class C {
  var a = t(1);
}

var c = new unmanaged C();
t(1) = 2;
var d = new unmanaged C();

writeln(c);
writeln(d);

delete c;
delete d;
