var s = "[\\]^_`";

writeln(s.isAlpha());

var s2 = "AZ[\\]^_`az";

for i in 0..<s2.size {
  var c = s2[i];
  writeln(c.isAlpha());
}
