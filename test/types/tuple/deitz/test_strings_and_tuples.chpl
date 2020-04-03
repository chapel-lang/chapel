var s1: string;
var s2: string;

s1 = "AAA";
writeln((s1, s2));
s2 = s1;
writeln((s1, s2));
s1 = "BBB";
writeln((s1, s2));

proc foo(s: string) {
  writeln((s1, s2, s));
  s1 = "CCC";
  writeln((s1, s2, s));
}
foo(s1);

var t: (string, string);
t = (s1, s2);
writeln((s1, s2, t));
s1 = "DDD";
writeln((s1, s2, t));

var tt = (s1, s2);
writeln((s1, s2, tt));
s1 = "EEE";
writeln((s1, s2, tt));

proc foo(t) {
  writeln((s1, s2, t));
  s1 = "FFF";
  writeln((s1, s2, t));
}
foo((s1, s2));

var A: [1..3] int;
A = 1..3;
writeln(A);

{
  var t = (A, A);
  writeln((A, t));
  A = 4..6;
  writeln((A, t));
  t(0)(1) = 7;
  writeln((A, t));
}

