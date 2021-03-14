record myR {
  var s: string;
}

class myC {
  var r: myR;
}


var c  = new unmanaged myC();
var s1 = "hi";

writeln((c,s1));
c.r.s = s1;
writeln((c,s1));

delete c;

