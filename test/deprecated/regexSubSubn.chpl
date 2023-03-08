use Regex;

var myStr = "AABBCC";
var r = new regex("A.*B");

writeln(r.sub(myStr, "x"));
writeln(r.subn(myStr, "x"));
