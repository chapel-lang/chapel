use Time;

config const timing = true;
config const n = 100000;

var search: string;
for i in 1..n do
  search += "search";

// substring
var tSubstring: Timer;
if timing then tSubstring.start();
for i in 1..n do
  var substr = search[1..i];
if timing then tSubstring.stop();

// count
var tCount: Timer;
if timing then tCount.start();
var count = search.count("sea");
if timing then tCount.stop();

// replace
var tReplace: Timer;
if timing then tReplace.start();
search.replace("searchsearchsearchsearch", "sea");
if timing then tReplace.stop();

if timing {
  writeln("substring: ", tSubstring.elapsed());
  writeln("count: ", tCount.elapsed());
  writeln("replace: ", tReplace.elapsed());
}
if count == n then
  writeln("SUCCESS");
