var hi = "  hello world   	";
var ec = "  être coincé   	";
var spaces = "\n \t  \r";
var letters = "abcdcba";
var lettres = "àbçdçbà";

on Locales[numLocales-1] {
  writeln('"', hi, '"');
  writeln('"', hi.strip(), '"');
  writeln('"', hi.strip(leading=false), '"');
  writeln('"', hi.strip(trailing=false), '"');
  writeln('"', hi.strip(leading=false, trailing=false), '"');

  writeln('"', ec, '"');
  writeln('"', ec.strip(), '"');
  writeln('"', ec.strip(leading=false), '"');
  writeln('"', ec.strip(trailing=false), '"');
  writeln('"', ec.strip(leading=false, trailing=false), '"');

  writeln('"', spaces.strip(), '"');
  writeln('"', spaces.strip(leading=false), '"');
  writeln('"', spaces.strip(trailing=false), '"');

  writeln('"', letters.strip("a"), '"');
  writeln('"', letters.strip("ba"), '"');
  writeln('"', letters.strip("bca"), '"');
  writeln('"', letters.strip("bcda"), '"');

  writeln('"', lettres.strip("à"), '"');
  writeln('"', lettres.strip("bà"), '"');
  writeln('"', lettres.strip("bçà"), '"');
  writeln('"', lettres.strip("bçdà"), '"');
}
