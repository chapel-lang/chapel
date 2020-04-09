var foo = "Foo foo Foo foo";
var summer = "Été été Été été";
var nums = "11122233344411111";
var hello = "Hello, world!";
var stuck = "Être coincé";
var x = "x";
var a = "à";

on Locales[numLocales-1] {
  writeln(foo.replace("Foo", "Baz"));
  writeln(foo.replace("foo", "baz"));
  writeln(foo.replace("Foo", "Baz").replace("foo", "baz"));

  writeln(summer.replace("Été", "Œf"));
  writeln(summer.replace("été", "œf"));
  writeln(summer.replace("Été", "Œf").replace("été", "œf"));

  writeln(nums.replace("1", "A", 4));
  writeln(nums.replace("2", "B", -1));
  writeln(nums.replace("3", "C", 1));
  writeln(nums.replace("4", "D", 0));

  writeln(nums.replace("1", "À", 4));
  writeln(nums.replace("2", "Ê", -1));
  writeln(nums.replace("3", "Í", 1));
  writeln(nums.replace("4", "Ô", 0));

  writeln(hello.replace("world", "chapel"));

  writeln(stuck.replace("coincé", "égal"));

  writeln(x.replace("x", "yy"));
  writeln(x.replace("x", "xx"));

  writeln(a.replace("à", "çç"));
  writeln(a.replace("à", "àà"));
}
