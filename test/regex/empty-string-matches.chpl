// Verify that `matches` produces at most one match
// when `text` is an empty string.

use Regex;

var str = "";

writeln(new regex("").matches(str));
writeln(new regex("^").matches(str));
writeln(new regex("$").matches(str));
writeln(new regex("^$").matches(str));
writeln(new regex(".*").matches(str));
