var x = "xxxxx";
var a = "ààààà";
var hello = "hello, world";
var stuck = "être coincé";
var foo = "foo bar";
var summer = "été élongé";

on Locales[numLocales-1] {
  writeln(x.startsWith("y"));
  writeln(x.startsWith("y", "x"));
  writeln(x.endsWith("xxxxxx"));
  writeln(x.endsWith("xxxxxx", "x"));

  writeln(a.startsWith("è"));
  writeln(a.startsWith("è", "à"));
  writeln(a.endsWith("àààààà"));
  writeln(a.endsWith("àààààà", "à"));

  writeln(hello.startsWith("world"));
  writeln(hello.startsWith("hello"));
  writeln(hello.endsWith("hello"));
  writeln(hello.endsWith("world"));

  writeln(stuck.startsWith("coincé"));
  writeln(stuck.startsWith("être"));
  writeln(stuck.endsWith("être"));
  writeln(stuck.endsWith("coincé"));

  writeln(foo.startsWith("baz", "foo"));
  writeln(foo.endsWith("baz", "bar"));

  writeln(summer.startsWith("élonger", "été"));
  writeln(summer.endsWith("élonger", "élongé"));
}
