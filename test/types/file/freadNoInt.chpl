var myInt: int;
var f: file = new file(filename = "freadNoInt.txt", mode = FileAccessMode.read);

f.open();
f.read(myInt);
