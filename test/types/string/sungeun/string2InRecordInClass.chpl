record myR {
  var s: string;
}
class myC {
  var r: myR;
}
var c = new owned myC();
var s1 = "hi";

writeln(c);
c.r.s = s1;
writeln(c);
