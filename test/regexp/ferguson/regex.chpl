use Regexp;

writeln("Compile/Delete");
{
  var r = compile("[a-z]+");
}

writeln("Search 1");
{
  var r = compile("[a-z]+");

  var str = " test ";
  var match = r.search(str);

  writeln(match);

  writeln(str[match]);
}

writeln("Search 2");
{
  var r = compile("(t)[a-z]+");

  var str = " test ";
  var cap:string;
  var match = r.search(str, cap);

  writeln(match);
  writeln(cap);

  writeln(str[match]);
}

writeln("Search 3");
{
  var r = compile("(t)[a-z]+");

  var str = " test ";
  var cap:reMatch;
  var match = r.search(str, cap);

  writeln(match);
  writeln(cap);

  writeln(str[match]);
}

writeln("Search 4");
{
  var r = compile("([a-z]+)");

  var str = " test ";
  var cap:reMatch;
  var match = r.search(str, cap);

  writeln(match);
  writeln(cap);

  writeln(str[match]);
}

writeln("Search 5");
{
  var r = compile("([0-9]+)");

  var str = " 57 ";
  var cap:int;
  var match = r.search(str, cap);

  writeln(match);
  writeln(cap);

  writeln(str[match]);
}

writeln("Match 1");
{
  var r = compile("[a-z]+");

  var str = "test ";
  var match = r.match(str);

  writeln(match);

  writeln(str[match]);
}

writeln("Match 2");
{
  var r = compile("(t)[a-z]+");

  var str = "test ";
  var cap:string;
  var match = r.match(str, cap);

  writeln(match);
  writeln(cap);

  writeln(str[match]);
}

writeln("Match 3");
{
  var r = compile("(t)[a-z]+");

  var str = " test ";
  var cap:string;
  var match = r.match(str, cap);

  writeln(match);
  writeln(cap);

  writeln(str[match]);
}

writeln("Split 1");
{
  var split = compile("\\W+");
  for s in split.split("Words, words, words.") {
    writeln(s);
  }
}

writeln("Split 2");
{
  var split = compile("(\\W+)");
  for s in split.split("Words, words, words.") {
    writeln(s);
  }
}

writeln("Split 3");
{
  var split = compile("\\W+");
  for s in split.split("...words, words...") {
    writeln(s);
  }
}

writeln("Split 4");
{
  var split = compile("(\\W+)");
  for s in split.split("...words, words...") {
    writeln(s);
  }
}

writeln("Split 5");
{
  var split = compile("\\W+");
  for s in split.split("Words, words, words.", 1) {
    writeln(s);
  }
}

writeln("Split 6");
{
  var split = compile("\\W+");
  for s in split.split("Words, words, words.", 2) {
    writeln(s);
  }
}


writeln("Matches 1");
{
  var re = compile("\\w+");
  var str = "Words, words, word.";
  for s in re.matches(str, 0) {
    writeln(str[s[1]]);
  }
}
writeln("Matches 2");
{
  var re = compile("(w)(\\w+)");
  var str = "Words, words, word.";
  for s in re.matches(str, 2) {
    writeln(str[s[1]]);
    writeln(str[s[2]]);
    writeln(str[s[3]]);
  }
}

writeln("sub 1");
{
  var re = compile("(w)(\\w+)");
  var str = "Words, words, word.";
  writeln(re.sub("\\1-\\2", str, true));
}

writeln("sub 2");
{
  var re = compile("(w)(\\w+)");
  var str = "Words, words, word.";
  writeln(re.subn("\\1-\\2", str, true));
}

writeln("sub 3");
{
  var re = compile("(w)(\\w+)");
  var str = "Words, words, word.";
  writeln(re.sub("\\1-\\2", str, false));
}
