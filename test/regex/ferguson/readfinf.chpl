// Do not change this line: it is used for .good. //
// Verify that readf-ing an empty string does not advance the channel.

use Regex, IO;

var ch = openReader("readfinf.chpl");

for i in 1..4 {
  const ok = ch.readf("%//");
  writeln(ok, " ", ch.offset());
}

writeln();

for i in 1..4 {
  var str = "hi";
  const ok = ch.readf("%/()/", str);
  writeln(ok, " <", str, "> ", ch.offset());
}

writeln();

for i in 1..4 {
  var str = "hi";
  const ok = ch.readf("%/(.)/", str);
  writeln(ok, "<", str, ">", ch.offset());
}

writeln();

for i in 1..4 {
  const ok = ch.readf("");
  writeln(ok, " ", ch.offset());
}
