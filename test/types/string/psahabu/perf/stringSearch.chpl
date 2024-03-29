use Time;
use IO;

config const timing = true;
config const n = 10000000;
config const sourcePath = "moby.txt";

var mobyFile = open(sourcePath, ioMode.r);
var mobyReader = mobyFile.reader(locking=false);

var Passage: [1..n] string;
var word: string;
for i in 1..n {
  var read = mobyReader.read(word);
  if !read {
    mobyReader = mobyFile.reader(locking=false);
    continue;
  }
  Passage[i] = word;
}

// startsWith
var tStartsWith: stopwatch;
if timing then tStartsWith.start();
for i in 1..n {
  var s = Passage[i];
  s.startsWith("dis", "ir", "re", "un");
}
if timing then tStartsWith.stop();

// endsWith
var tEndsWith: stopwatch;
if timing then tEndsWith.start();
for i in 1..n {
  var s = Passage[i];
  s.endsWith("ed", "ing", "ly", "es");
}
if timing then tEndsWith.stop();

// find
var tFind: stopwatch;
if timing then tFind.start();
for i in 1..n {
  var s = Passage[i];
  s.find("white");
}
if timing then tFind.stop();

// rfind
var tRFind: stopwatch;
if timing then tRFind.start();
for i in 1..n {
  var s = Passage[i];
  s.rfind("whale");
}
if timing then tRFind.stop();

if timing {
  writeln("startsWith: ", tStartsWith.elapsed());
  writeln("endsWith: ", tEndsWith.elapsed());
  writeln("find: ", tFind.elapsed());
  writeln("rfind: ", tRFind.elapsed());
}
writeln("SUCCESS");
