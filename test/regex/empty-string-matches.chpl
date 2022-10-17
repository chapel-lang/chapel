// Verify that `matches` produces at most one match
// when `text` is an empty string.

use Regex;

var str = "";

writeln(compile("").matches(str));
writeln(compile("^").matches(str));
writeln(compile("$").matches(str));
writeln(compile("^$").matches(str));
writeln(compile(".*").matches(str));
