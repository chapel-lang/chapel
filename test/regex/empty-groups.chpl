use Regex;

var r = new regex("(a)|(b)");
var s = "aabb";
for m in r.split(s) do writeln("split='",m, "'");
