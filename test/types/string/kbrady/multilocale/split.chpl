var csv = "1,'abc foo',1.00,$123";
var fox = "The quick brown fox jumps over the lazy dog";

on Locales[numLocales-1] {
  var x = csv.split(",");
  writeln(x);
  for y in x do writeln('"', y, '"');
}
