use Time;
use IO;

enum types { asciiString, utf8String, byteString }

config param testType = types.asciiString;

type dataType = if testType == types.byteString then bytes else string;

config const timing = true;
config const n = 10000000;
config const sourcePath = if testType == types.utf8String
                          then "origin.txt"
                          else "moby.txt";

const startsWithNeedles;
const endsWithNeedles;
const findNeedle;
const rfindNeedle;

if testType == types.utf8String {
  startsWithNeedles = ("me":dataType,
                       "ons":dataType,
                       "dû":dataType,
                       "un":dataType);
  endsWithNeedles = ("is":dataType,
                     "ôt":dataType,
                     "le":dataType,
                     "es":dataType);
  findNeedle = "été":dataType;
  rfindNeedle = "crois":dataType;

}
else {
  startsWithNeedles = ("dis":dataType,
                       "ir":dataType,
                       "re":dataType,
                       "un":dataType);
  endsWithNeedles = ("ed":dataType,
                     "ing":dataType,
                     "ly":dataType,
                     "es":dataType);
  findNeedle = "white":dataType;
  rfindNeedle = "whale":dataType;
}

var mobyFile = open(sourcePath, iomode.r);
var mobyReader = mobyFile.reader();

var Passage: [1..n] dataType;
var word: dataType;
var totalNumBytes = 0;
for i in 1..n {
  var read = mobyReader.read(word);
  if !read {
    mobyReader = mobyFile.reader();
    continue;
  }
  Passage[i] = word;
  totalNumBytes += word.numBytes;
}

if timing {
  writeln("Read ", totalNumBytes, " bytes");
}

// startsWith
var tStartsWith: Timer;
if timing then tStartsWith.start();
for i in 1..n {
  var s = Passage[i];
  s.startsWith((...startsWithNeedles));
}
if timing then tStartsWith.stop();

// endsWith
var tEndsWith: Timer;
if timing then tEndsWith.start();
for i in 1..n {
  var s = Passage[i];
  s.endsWith((...endsWithNeedles));
}
if timing then tEndsWith.stop();

// find
var tFind: Timer;
if timing then tFind.start();
for i in 1..n {
  var s = Passage[i];
  s.find(findNeedle);
}
if timing then tFind.stop();

// rfind
var tRFind: Timer;
if timing then tRFind.start();
for i in 1..n {
  var s = Passage[i];
  s.rfind(rfindNeedle);
}
if timing then tRFind.stop();

if timing {
  writeln("startsWith: ", (totalNumBytes/1_000_000)/tStartsWith.elapsed());
  writeln("endsWith: ", (totalNumBytes/1_000_000)/tEndsWith.elapsed());
  writeln("find: ", (totalNumBytes/1_000_000)/tFind.elapsed());
  writeln("rfind: ", (totalNumBytes/1_000_000)/tRFind.elapsed());
}
writeln("SUCCESS");
