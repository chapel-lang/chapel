var x: int;
var y: string;
var z: bool;

var f: file = new file("fileIO.in.txt", FileAccessMode.read);

f.open();
f.readln(x);
f.readln(y);
f.readln(z);
f.close();

f = new file("fileIO.out.txt", FileAccessMode.write);

f.open();
f.writeln("(x,y,z) is: ", (x,y,z));
f.close();
