config const n = 100;
config const debug = false;

var f = openmem();
var r = f.reader(kind=ionative);
var w = f.writer();

var numChars = 0;
for i in 1..n {
  for j in 1..#i do w.write(j%10);
  w.writeln();
  numChars += i + 1;
}

var data : [1..numChars] uint(8);

var total : int = 0;
for i in 1..n {
  var numRead : int;
  r.readline(data, numRead, total+1);
  total += numRead;
}

var base = 1;
var numWrong = 0;
for i in 1..n {
  for j in 1..#i do
    if data[base+j-1] != ascii((j%10):string) then numWrong += 1;
  if data[base+i] != 0x0A then numWrong += 1;
  base += i+1;
}
if numWrong != 0 then writeln(numWrong, " out of ", numChars, " were read incorrectly");

if total != numChars then writeln("FAILURE: read ", total, ", but expected ", numChars);
else writeln("SUCCESS");
