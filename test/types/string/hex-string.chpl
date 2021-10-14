config type t = string;
param a = "12":t;

param pstr1 = "\x30":t;
param pstr2 = pstr1 + a;

var vstr1 = pstr1;
var vstr2 = vstr1 + a;

writeln(pstr2);
writeln(vstr2); // "012"
writeln(pstr2 == vstr2);

