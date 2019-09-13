var a1 = 1..6 by -1;
var a2 = 1..6:int(64) by -1;
var a3 = 1..6:uint by -1;
var a4 = 1..6:uint(64) by -1;

var i1: int;
var i2: int(64);
var i3: uint;
var i4: uint(64);

for i in a1 do
  i1 = i1 + i;

for i in a2 do
  i2 = i2 + i;

for i in a3 do
  i3 = i3 + i;

for i in a4 do
  i4 = i4 + i;

writeln((a1, a2, a3, a4));
writeln((i1, i2, i3, i4));

for i in a1 do
  writeln(i);

for i in a2 do
  writeln(i);

for i in a3 do
  writeln(i);

for i in a4 do
  writeln(i);
