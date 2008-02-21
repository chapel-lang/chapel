var myFloat: real;
var f: file = new file(filename = "freadNoFloat.txt", mode = FileAccessMode.read);

f.open();
f.read(myFloat);
