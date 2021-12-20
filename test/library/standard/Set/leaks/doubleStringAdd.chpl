use Set;
record R {
  var a: string;
}

var s = new set(R);
var a = new R("1");

s.add(a);
s.add(a);
writeln(s);
