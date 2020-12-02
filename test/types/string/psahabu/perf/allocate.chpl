use Time;

enum types { asciiString, utf8String, byteString }

config param testType = types.asciiString;

type dataType = if testType == types.byteString then bytes else string;

config const n = 10000000;
config const timing = true;

// concatenate
var tConcat: Timer;
var concat: dataType;
if timing then tConcat.start();
if testType == types.utf8String {
  for i in 1..n do
    concat = "concat concatenation'un kısası" + i:string;
}
else if testType == types.byteString {
  for i in 1..n do
    concat = b"concat's short for concatenation" + i:bytes;
}
else {
  for i in 1..n do
    concat = "concat's short for concatenation" + i:string;
}
if timing then tConcat.stop();

// append
var tAppend: Timer;
var append: dataType;
if timing then tAppend.start();
if testType == types.utf8String {
  for i in 1..n do
    append += "añade";
}
else if testType == types.byteString {
  for i in 1..n do
    append += b"append";
}
else {
  for i in 1..n do
    append += "append";
}
if timing then tAppend.stop();

// copy
var tCopy: Timer;
var keepAlive: int;
var copied = if testType == types.utf8String then "mañana"
             else if testType == types.byteString then b"copycat"
             else "copycat";

if timing then tCopy.start();
if testType == types.byteString {
  for i in 1..n {
    var copy = createBytesWithNewBuffer(copied);
    keepAlive += copy.buffLen;
  }
}
else {
  for i in 1..n {
    var copy = createStringWithNewBuffer(copied);
    keepAlive += copy.buffLen;
  }
}
if timing then tCopy.stop();

// iterate
var tIterate: Timer;
if timing then tIterate.start();
var count = 0;
for i in append do
  count += 1;
if timing then tIterate.stop();

if timing {
  writeln("concat: ", tConcat.elapsed());
  writeln("append: ", tAppend.elapsed());
  writeln("copy: ", tCopy.elapsed());
  writeln("iterate: ", tIterate.elapsed());
}
var numRefChars = if testType == types.utf8String then 5 else 6;
if count == n * numRefChars then
  writeln("SUCCESS");
