var a:int(32) = 1;
var count:int(64) = 2;
for i in a..#count {
  writeln(i, " : ", i.type:string);
}
var r = a..#count;
writeln(r, " : ", r.type:string);
