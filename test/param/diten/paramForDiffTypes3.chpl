param low: int(32) = 1;
param high: int(64) = 2;

var x = low;
var y = high;
var r = low..high;
var t = x..y;
writeln("r ", r, " : ", r.type:string);
writeln("t ", t, " : ", t.type:string);

for param i in low..high {
  writeln("p ", (i, i.type:string));
}

for i in low..high {
  writeln("v ", (i, i.type:string));
}

for i in x..y {
  writeln("u ", (i, i.type:string));
}
