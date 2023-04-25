use Regex;   // vass: deprecated for 1.29.0

writeln("Search");
{
var s = "contains";
var b = b"contains";
// check deprecation of casts string|bytes --> regex
var rs = s:regex(string);
var rb = b:regex(bytes);

writeln(rs.search("contains regular expression"));
writeln(rb.search(b"contains regular expression"));

writeln(rs.search("doesn't contain regular expression"));
writeln(rb.search(b"doesn't contain regular expression"));
}
