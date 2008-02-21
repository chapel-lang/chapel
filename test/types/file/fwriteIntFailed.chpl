var myInt = 123;
var f: file = new file(filename = "fwriteIntFailed.txt", mode = FileAccessMode.read);

f.open();
f.write(myInt);
