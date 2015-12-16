var foo = "Foo foo Foo foo";
var nums = "11122233344411111";
var hello = "Hello, world!";
var x = "x";

writeln(foo.replace("Foo", "Baz"));
writeln(foo.replace("foo", "baz"));
writeln(foo.replace("Foo", "Baz").replace("foo", "baz"));

writeln(nums.replace("1", "A", 4));
writeln(nums.replace("2", "B", -1));
writeln(nums.replace("3", "C", 1));
writeln(nums.replace("4", "D", 0));

writeln(hello.replace("world", "chapel"));

writeln(x.replace("x", "yy"));
writeln(x.replace("x", "xx"));
