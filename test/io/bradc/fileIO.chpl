var x: int;
var y: string;
var z: bool;

var r = open("fileIO.in.txt", mode.r).reader();

r.readln(x);
r.readln(y);
r.readln(z);
r.close();

var w = open("fileIO.out.txt", mode.w).writer();

w.writeln("(x,y,z) is: ", (x,y,z));
w.close();
