class myC {
  var s: string;
}

var c  = new unmanaged myC();
var s1 = "hi";

writeln((c,s1));
c.s = s1;
writeln((c,s1));

delete c;

