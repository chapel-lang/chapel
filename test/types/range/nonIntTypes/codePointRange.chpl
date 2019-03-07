var lo = 1: codePointIndex;
var hi = 3: codePointIndex;

var r = lo..hi;
writeln(r);
writeln(r.type:string);
for i in r do
  writeln(i);
var r2 = lo..4:codePointIndex by 2;
writeln(r2);
writeln(r2.type:string);
for i in r2 do
  writeln(i);
