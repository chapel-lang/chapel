use IO;

var x: int;
var y: string;
var z: bool;

var r = open("fileIO.in.txt", iomode.r).reader();

r.readln(x);
r.readln(y);
r.readln(z);
r.close();

var w = open("fileIO.out.txt", iomode.cw).writer();

w.writeln("(x,y,z) is: ", (x,y,z));
w.close();
