use Regex;

config type t = string;

writeln("Compile/Delete");
{
  var r = new regex("[a-z]+":t);
}

writeln("Search 1");
{
  var r = new regex("[a-z]+":t);

  var str = " test ":t;
  {
    var match = r.search(str);

    writeln(match);

    writeln(str[match]);
  }
  {
    var match = str.find(r);

    writeln(match);

  }
}

writeln("Search 2");
{
  var r = new regex("(t)[a-z]+":t);

  var str = " test ":t;
  var cap:t;
  {
    var match = r.search(str, cap);

    writeln(match);
    writeln(cap);

    writeln(str[match]);
  }
  {
    var match = str.find(r);

    writeln(match);
  }
}

writeln("Search 3");
{
  var r = new regex("(t)[a-z]+":t);

  var str = " test ":t;
  var cap:regexMatch;
  {
    var match = r.search(str, cap);

    writeln(match);
    writeln(cap);

    writeln(str[match]);
  }
  {
    var match = str.find(r);

    writeln(match);
  }
}

writeln("Search 4");
{
  var r = new regex("([a-z]+)":t);

  var str = " test ":t;
  var cap:regexMatch;
  {
    var match = r.search(str, cap);

    writeln(match);
    writeln(cap);

    writeln(str[match]);
  }{
    var match = str.find(r);

    writeln(match);
  }
}

writeln("Search 5");
{
  var r = new regex("([0-9]+)":t);

  var str = " 57 ":t;
  var cap:int;
  {
    var match = r.search(str, cap);

    writeln(match);
    writeln(cap);

    writeln(str[match]);
  }
  {
    var match = str.find(r);

    writeln(match);
  }
}

writeln("Match 1");
{
  var r = new regex("[a-z]+":t);

  var str = "test ":t;
  {
    var match = r.match(str);

    writeln(match);

    writeln(str[match]);
  }
  {
    var match = str.startsWith(r);

    writeln(match);
  }
}

writeln("Match 2");
{
  var r = new regex("(t)[a-z]+":t);

  var str = "test ":t;
  var cap:t;
  {
    var match = r.match(str, cap);

    writeln(match);
    writeln(cap);

    writeln(str[match]);
  }
  {
    var match = str.startsWith(r);

    writeln(match);
  }
}

writeln("Match 3");
{
  var r = new regex("(t)[a-z]+":t);

  var str = " test ":t;
  var cap:t;
  {
    var match = r.match(str, cap);

    writeln(match);
    writeln(cap);

    writeln(str[match]);
  }
  {
    var match = str.startsWith(r);

    writeln(match);
  }
}

writeln("Split 1");
{
  var split = new regex("\\W+":t);
  var str = "Words, words, words.":t;
  for s in split.split(str) {
    writeln(s);
  }
  for s in str.split(split) {
    writeln(s);
  }
}

writeln("Split 2");
{
  var split = new regex("(\\W+)":t);
  var str = "Words, words, words.":t ;
  for s in split.split(str) {
    writeln(s);
  }
  for s in str.split(split) {
    writeln(s);
  }
}

writeln("Split 3");
{
  var split = new regex("\\W+":t);
  var str = "...words, words...":t;
  for s in split.split(str) {
    writeln(s);
  }
  for s in str.split(split) {
    writeln(s);
  }
}

writeln("Split 4");
{
  var split = new regex("(\\W+)":t);
  var str = "...words, words...":t;
  for s in split.split(str) {
    writeln(s);
  }
  for s in str.split(split) {
    writeln(s);
  }
}

writeln("Split 5");
{
  var split = new regex("\\W+":t);
  var str = "Words, words, words.":t;
  for s in split.split(str, 1) {
    writeln(s);
  }
  for s in str.split(split, 1) {
    writeln(s);
  }
}

writeln("Split 6");
{
  var split = new regex("\\W+":t);
  var str = "Words, words, words.":t;
  for s in split.split(str, 2) {
    writeln(s);
  }
  for s in str.split(split, 2) {
    writeln(s);
  }
}

writeln("Replace 1");
{
  var re = new regex("(w)(\\w+)":t);
  var str = "Words, words, word.":t;
  writeln(str.replace(re, "\\1-\\2":t, count=-1));
}

writeln("Replace 2");
{
  var re = new regex("(w)(\\w+)":t);
  var str = "Words, words, word.":t;
  writeln(str.replaceAndCount(re, "\\1-\\2":t, count=-1));
}

writeln("Replace 3");
{
  var re = new regex("(w)(\\w+)":t);
  var str = "Words, words, word.":t;
  writeln(str.replace(re, "\\1-\\2":t, count=1));
}
