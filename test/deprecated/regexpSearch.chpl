use Regex;

config type t1 = string;

{
  writeln("search string");

  var str = " test ":t1;
  var r = compile("(t)[a-z]+":t1);
  var cap:regexMatch;

  var match = str.search(needle=r, cap);

  writeln(match);
  writeln(cap);

  writeln(str[match]);
}

config type t2 = bytes;

{
  writeln("search bytes");

  var b = b" test ":t2;
  var r = compile("(t)[a-z]+":t2);
  var cap:regexMatch;

  var match = b.search(needle=r, cap);

  writeln(match);
  writeln(cap);

  writeln(b[match]);
}
