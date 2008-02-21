var myInt: int;
var f: file = new file(filename = "anyFile.txt", mode = FileAccessMode.q);

f.open();
f.write(myInt);
