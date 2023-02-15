use Regex;
use Time;

config const useGlobal = false;
config const nonMatchingIntervalLen = 3;
config const matchingIntervalLen = 3;
config const repeatCount = 4;
config const replacementCount = repeatCount;
config const verify = true;
config const printResult = true;
config const printTime = false;
config const numIter = 10;

if useGlobal then assert(replacementCount == repeatCount);
if replacementCount != repeatCount {
  writeln("Warning: can't use `useGlobal` to compare performance");
}

var r = compile("1+");
var s = ("a"*nonMatchingIntervalLen+"1"*matchingIntervalLen)*repeatCount;
var t: stopwatch;

var ret: (string, int);
for i in 0..numIter {
  if i==1 then t.start();
  if useGlobal then
    ret = s.replaceAndCount(r, "0");
  else
    ret = s.replaceAndCount(r, "0", count=replacementCount);
}
t.stop();

if printResult then writeln(ret);

if verify {
  if repeatCount == replacementCount {
    assert(ret == s.replaceAndCount(r, "0"));
  }
  else {
    writeln("Can't verify the result");
  }
}

if printTime then writeln("Time per iteration (s): ", t.elapsed()/numIter);
