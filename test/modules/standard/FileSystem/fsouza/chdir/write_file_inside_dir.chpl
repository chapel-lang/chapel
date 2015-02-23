use FileSystem;
var directory = "./destination";
here.chdir(directory);
var w = open("file.txt", iomode.cwr).writer();
w.writeln("hello");
w.close();
