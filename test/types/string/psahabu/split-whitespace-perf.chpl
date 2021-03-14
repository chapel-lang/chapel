use Time;

enum types { asciiString, utf8String, byteString }

config param testType = types.asciiString;

type dataType = if testType == types.byteString then bytes else string;

config const n = 2000000;
config const timing = true;

var very_long: dataType;

// both weird french and english versions are 27 bytes, and 6 splits. If you
// change the text, make sure to have something close for better comparison
if testType == types.utf8String {
  for i in 1..n {
    very_long += "ce est un texte très long ";
  }
}
else {
  for i in 1..n {
    very_long += "this is a very long string ":dataType;
  }
}

var i: int;
var t: Timer;
t.start();
for s in very_long.split() {
  i += 1;
}
t.stop();

if timing then
  writeln("Time: ", t.elapsed());
if i == 6 * n then
  writeln("SUCCESS");
