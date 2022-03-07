use Regex;

config type t = string;

writeln("Search 1");
{
  var r = compile("[a-z]+":t);

  var str = " test ":t;
  {
    var match = r.search(str);

    writeln(match.size);
    writeln(match.offset);
  }
}

