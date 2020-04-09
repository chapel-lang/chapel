class C {
  var x = 12;
}

record R {
  var y = 14;
}

var c = new unmanaged C();
var r = new R();

writeln(c);
writeln(r);

var c2 = c;
var r2 = r;

writeln(c2);
writeln(r2);

c2.x -= 1;
r2.y -= 1;

writeln(c);
writeln(r);
writeln(c2);
writeln(r2);

delete c;
