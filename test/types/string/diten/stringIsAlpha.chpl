var s = "[\\]^_`";

writeln(s.isAlpha());

var s2 = "AZ[\\]^_`az";

for i in 1..s2.length {
  var c = s2[i];
  writeln(c.isAlpha());
}
