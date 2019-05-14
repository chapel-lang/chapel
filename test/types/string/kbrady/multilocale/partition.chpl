var middle = "foo-bar baz gob";
var left = "-foo bar baz gob";
var right = "foo bar baz gob-";
var two = "foo-bar-baz gob";
var nosep = "foo bar baz gob";

var middle2 = "à€ça été œf";
var left2 = "€à ça été œf";
var right2 = "à ça été œf€";
var two2 = "à€ça€été œf";
var nosep2 = "à ça été œf";

on Locales[numLocales-1] {
  writeln(middle.partition("-"));
  writeln(left.partition("-"));
  writeln(right.partition("-"));
  writeln(two.partition("-"));
  writeln(nosep.partition("-"));

  writeln(middle2.partition("€"));
  writeln(left2.partition("€"));
  writeln(right2.partition("€"));
  writeln(two2.partition("€"));
  writeln(nosep2.partition("€"));
}
