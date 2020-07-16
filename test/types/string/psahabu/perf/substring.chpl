use Time;

enum types { asciiString, utf8String, byteString }

config param testType = types.asciiString;

type dataType = if testType == types.byteString then bytes else string;

config const timing = true;
config const n = 100000;

var search: dataType;
if testType == types.utf8String {
  for i in 1..n do
    search += "había";
}
else {
  for i in 1..n do
    search += "search":dataType;
}

// substring
var tSubstring: Timer;
if timing then tSubstring.start();
for i in 1..n do
  var substr = search[1..i];
if timing then tSubstring.stop();

// count
var tCount: Timer;
var count;
if timing then tCount.start();
if testType == types.utf8String {
  count = search.count("ía");
}
else if testType == types.asciiString {
  count = search.count("sea");
}
else {
  count = search.count(b"sea");
}
if timing then tCount.stop();

// replace
var tReplace: Timer;
if timing then tReplace.start();
if testType == types.utf8String {
  search.replace("habíahabíahabíahabía", "ía");
}
else if testType == types.asciiString {
  search.replace("searchsearchsearchsearch", "sea");
}
else {
  search.replace(b"searchsearchsearchsearch", b"sea");
}
if timing then tReplace.stop();

if timing {
  writeln("substring: ", tSubstring.elapsed());
  writeln("count: ", tCount.elapsed());
  writeln("replace: ", tReplace.elapsed());
}
if count == n then
  writeln("SUCCESS");
