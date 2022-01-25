use Regex;

config type t = string;

var locales1 = Locales[numLocales - 1];

proc preventRvf(x) {
  /* this is always true but makes sure rvf won't fire */
  assert(x.locale.id >= 0);
}

proc searchL() {
  var a:t;
  var b:t;
  var s = "aaabb":t;
  var r = compile("(a+)(b+)":t);
  var match = r.search(s, a, b);
  writeln("searchL");
  writeln(match);
  writeln(a);
  writeln(b);
  writeln();
}

proc searchR(param rvf=true) {
  var a:t;
  var b:t;
  var s = "aaabb":t;
  var r = compile("(a+)(b+)":t);
  on locales1 {
    if !rvf then preventRvf(r);
    var match = r.search(s, a, b);
    writef("searchR(rvf=%t)\n", rvf);
    writeln(match);
    writeln(a);
    writeln(b);
    writeln();
  }
}

proc splitL() {
  var s = "abaabaaab":t;
  var r = compile("(a+)":t);
  writeln("splitL");
  for match in r.split(s) do
    writeln(match);
  writeln();
}

proc splitR(param rvf=true) {
  var s = "abaabaaab":t;
  var r = compile("(a+)":t);
  on locales1 {
    if !rvf then preventRvf(r);
    writef("splitR(rvf=%t)\n", rvf);
    for match in r.split(s) do
      writeln(match);
    writeln();
  }
}

proc matchesL() {
  var s = "abaabaaab":t;
  var r = compile("(a+)(b)":t);
  writeln("matchesL");
  for match in r.matches(s, numCaptures=2) do
    writeln(match);
  writeln();
}

proc matchesR(param rvf=true) {
  var s = "abaabaaab":t;
  var r = compile("(a+)(b)":t);
  on locales1 {
    if !rvf then preventRvf(r);
    writef("matchesR(rvf=%t)\n", rvf);
    for match in r.matches(s, numCaptures=2) do
      writeln(match);
    writeln();
  }
}

proc subL() {
  var s = "abaabaaab":t;
  var r = compile("a+":t);
  writeln("subL");
  writeln(r.sub("A":t, s));
  writeln();
}

proc subR(param rvf=true) {
  var s = "abaabaaab":t;
  var r = compile("a+":t);
  on locales1 {
    if !rvf then preventRvf(r);
    writef("subR(rvf=%t)\n", rvf);
    writeln(r.sub("A":t, s));
    writeln();
  }
}

searchL();
searchR(rvf=true);
searchR(rvf=false);

splitL();
splitR(rvf=true);
splitR(rvf=false);

matchesL();
matchesR(rvf=true);
matchesR(rvf=false);

subL();
subR(rvf=true);
subR(rvf=false);
