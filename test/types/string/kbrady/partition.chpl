var middle = "foo-bar baz gob";
var left = "-foo bar baz gob";
var right = "foo bar baz gob-";
var two = "foo-bar-baz gob";
var none = "foo bar baz gob";

writeln(middle.partition("-"));
writeln(left.partition("-"));
writeln(right.partition("-"));
writeln(two.partition("-"));
writeln(none.partition("-"));
