var myInt = 123;
var f: file = new file(filename = "_test_freadIntFailed.txt", mode = FileAccessMode.write);

f.open();
f.read(myInt);
