use Time;

config const n = 10000000;
config const timing = true;

// concatenate
var tConcat: Timer;
if timing then tConcat.start();
var concat: string;
for i in 1..n do
  concat = "concat is short for " + "concatenation" + i:string;
if timing then tConcat.stop();

// append
var tAppend: Timer;
if timing then tAppend.start();
var append: string;
for i in 1..n do
  append += "append";
if timing then tAppend.stop();

// copy
var tCopy: Timer;
if timing then tCopy.start();
var keepAlive: int;
var copied = "copycat";
for i in 1..n {
  var copy = createStringWithNewBuffer(copied);
  keepAlive += copy.buffLen;
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
if count == n * 6 then
  writeln("SUCCESS");
