var x = "xxxxx";
var hello = "hello, world";
var foo = "foo bar";

writeln("find: ", hello.find(needle="world"));
writeln("rfind: ", hello.rfind(needle="world"));
writeln("count: ", x.count(needle="x"));
writeln("count: ", x.count(needle="x", region=0..2));
writeln("replace: ", foo.replace(needle="foo", "bar"));
