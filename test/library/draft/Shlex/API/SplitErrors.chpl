// Splitting strings with unbalanced quotes correctly give errors
use Shlex;
var src = "abc'def";
writeln(split(src));
