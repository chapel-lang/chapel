use Regex;

{
  writeln("search string");
  config type t = string;

  var str = " test ":t;
  var r = compile("(t)[a-z]+":t);
  var cap:regexMatch;

  var match = str.search(needle=r, cap);

  writeln(match);
  writeln(cap);

  writeln(str[match]);
}

{
  writeln("search bytes");
  config type t = bytes;

  var b = b" test ":t;
  var r = compile("(t)[a-z]+":t);
  var cap:regexMatch;

  var match = b.search(needle=r, cap);

  writeln(match);
  writeln(cap);

  writeln(b[match]);
}
