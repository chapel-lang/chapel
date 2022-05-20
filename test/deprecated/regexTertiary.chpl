use Regex;

writeln("Search");
{
var s = "contains";
var b = b"contains";

var rs = s:regex(string);
var rb = b:regex(bytes);

writeln("contains regular expression".search(rs));
writeln(b"contains regular expression".search(rb));

writeln("doesn't contain regular expression".search(rs));
writeln(b"doesn't contain regular expression".search(rb));
}

writeln("Match 1");
{
  var r = compile("[a-z]+":string);

  var text = "test ":string;
  {
    var match = r.match(text);

    writeln(match);

    writeln(text[match]);
  }
  {
    var match = text.match(r);

    writeln(match);
  }
}

writeln("Match 2");
{
  var r = compile("[a-z]+":bytes);

  var text = "test ":bytes;
  {
    var match = r.match(text);

    writeln(match);

    writeln(text[match]);
  }
  {
    var match = text.match(r);

    writeln(match);
  }
}

writeln("Matches 1");
{
  var text = "foo 123 bar 345":string;

  var r = Regex.compile("\\d+":string);

  for m in text.matches(r, maxmatches=1) {
    writeln(m);
  }
}

writeln("Matches 2");
{
  var text = "foo 123 bar 345":bytes;

  var r = Regex.compile("\\d+":bytes);

  for m in text.matches(r, maxmatches=1) {
    writeln(m);
  }
}

writeln("Split 1");
{
  var split = compile("\\W+":string);
  var text = "Words, words, words.":string;
  for s in text.split(pattern=split) {
    writeln(s);
  }
}

writeln("Split 2");
{
  var split = compile("\\W+":bytes);
  var text = "Words, words, words.":bytes;
  for s in text.split(pattern=split) {
    writeln(s);
  }
}

