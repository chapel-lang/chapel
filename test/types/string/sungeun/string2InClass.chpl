class myC {
  var s: string;
}

var c  = new unmanaged myC();
var s1 = "hi";

writeln(c);
c.s = s1;
writeln(c);

delete c;
