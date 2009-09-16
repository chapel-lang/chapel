var t = (1, 2);

class C {
  var a = t(1);
}

var c = new C();
t(1) = 2;
var d = new C();

writeln(c);
writeln(d);

delete c;
delete d;
