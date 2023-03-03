// check bytes (and while here string) casts to/from regular expressions

use Regex;

var s = "contains";
var b = b"contains";

var rcs = new regex(s);
var rcb = new regex(b);

writeln("contains regular expression".find(rcs));
writeln(b"contains regular expression".find(rcb));

writeln("doesn't contain regular expression".find(rcs));
writeln(b"doesn't contain regular expression".find(rcb));
