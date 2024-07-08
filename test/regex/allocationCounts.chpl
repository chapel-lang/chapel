use Regex;
use MemDiagnostics;

config type t = string;

var r = new regex("(a+)(b+)(c+)":t);

{
  writeln("Search no capture");
  startVerboseMem();
  var m = r.search("zzzzzaaabbbbbccccc":t);
  assert(m.matched);
  stopVerboseMem();
  writeln();
}

{
  writeln("Search with capture");
  startVerboseMem();
  var a:t;
  var b:t;
  var c:t;
  var m = r.search("zzzzzaaabbbbbccccc":t, a, b, c);
  assert(m.matched);
  stopVerboseMem();
  writeln((a, b, c));
  writeln();
}

{
  writeln("Iter Matches no capture");
  startVerboseMem();
  for (m,) in r.matches("abcaabbcc":t) {
    assert(m.matched);
  }
  stopVerboseMem();
  writeln();
}

{
  writeln("Iter Matches with capture");
  startVerboseMem();
  for (m, a, b, c) in r.matches("abcaabbcc":t, numCaptures=3) {
    assert(m.matched);
  }
  stopVerboseMem();
  writeln();
}
