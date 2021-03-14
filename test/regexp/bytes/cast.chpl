// check bytes (and while here string) casts to/from regular expressions

use Regexp;

var s = "contains";
var b = b"contains";

var rs = s:regexp(string);
var rb = b:regexp(bytes);

writeln("contains regular expression".search(rs));
writeln(b"contains regular expression".search(rb));

writeln("doesn't contain regular expression".search(rs));
writeln(b"doesn't contain regular expression".search(rb));

