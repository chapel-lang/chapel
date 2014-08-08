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
  r.readline(data[total+1..#(i+1)], numRead);
  total += numRead;
}

var base = 1;
for i in 1..n {
  for j in 1..#i do
    if data[base+j-1]-48 != (j%10) then writeln("FAILED CHAR: got ", data[base+j-1]-48, " but expected ", j%10);
  if data[base+i] != 0x0A then writeln("FAILED NEWLINE");
  base += i+1;
}

if total != numChars then writeln("FAILURE: read ", total, ", but expected ", numChars);
else writeln("SUCCESS");
