use Regex;
var re1 = Regex.compile("a.b", dotnl=true);
writeln(re1.match("a\nb").matched);
var re2 = Regex.compile("a.b", dotAll=true);
writeln(re2.match("a\nb").matched);
