use Regex;

var myStr = "AABBCC";
var r = compile("A.*B");

writeln(r.sub(myStr, "x"));
writeln(r.subn(myStr, "x"));
