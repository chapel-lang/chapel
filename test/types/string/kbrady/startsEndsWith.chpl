var x = "xxxxx";
var hello = "hello, world";
var foo = "foo bar";

writeln(x.startsWith("y"));
writeln(x.startsWith("y", "x"));
writeln(x.endsWith("xxxxxx"));
writeln(x.endsWith("xxxxxx", "x"));

writeln(hello.startsWith("world"));
writeln(hello.startsWith("hello"));
writeln(hello.endsWith("hello"));
writeln(hello.endsWith("world"));

writeln(foo.startsWith("baz", "foo"));
writeln(foo.endsWith("baz", "bar"));
