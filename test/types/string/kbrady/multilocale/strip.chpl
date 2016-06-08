var hi = "  hello world   	";
var spaces = "\n \t  \r";
var letters = "abcdcba";

on Locales[numLocales-1] {
  writeln('"', hi, '"');
  writeln('"', hi.strip(), '"');
  writeln('"', hi.strip(leading=false), '"');
  writeln('"', hi.strip(trailing=false), '"');
  writeln('"', hi.strip(leading=false, trailing=false), '"');

  writeln('"', spaces.strip(), '"');
  writeln('"', spaces.strip(leading=false), '"');
  writeln('"', spaces.strip(trailing=false), '"');

  writeln('"', letters.strip("a"), '"');
  writeln('"', letters.strip("ba"), '"');
  writeln('"', letters.strip("bca"), '"');
  writeln('"', letters.strip("bcda"), '"');
}
