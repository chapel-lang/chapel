config var l = 1:uint(8), u = 10:uint(8), s = -1;

for i in l..u by s do writeln(i);

var r2 = 0..u by s;
writeln(r2, " : ", r2.type:string);
