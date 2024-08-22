use Regex;

writeln("bbcc".replace(new regex("(b+)(c+)"), "\\1 \\2 \\0"));
