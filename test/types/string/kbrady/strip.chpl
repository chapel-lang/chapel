var hi = "  hello world   	";
writeln('"', hi, '"');
writeln('"', hi.strip(), '"');
writeln('"', hi.strip(leading=false), '"');
writeln('"', hi.strip(trailing=false), '"');
writeln('"', hi.strip(leading=false, trailing=false), '"');

var spaces = "\n \t  \r";
writeln('"', spaces.strip(), '"');
writeln('"', spaces.strip(leading=false), '"');
writeln('"', spaces.strip(trailing=false), '"');

var letters = "abcdcba";
writeln('"', letters.strip("a"), '"');
writeln('"', letters.strip("ba"), '"');
writeln('"', letters.strip("bca"), '"');
writeln('"', letters.strip("bcda"), '"');
