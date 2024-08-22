use Regex;

writeln("bb".replace(new regex("(b+)"), "a\\1a"));
writeln("bb".replace(new regex("(b+)", noCapture=false), "a\\1a"));
writeln("bb".replace(new regex("(b+)", noCapture=true), "a\\1a"));
