use FileSystem, IO;
var directory = "./destination";
here.chdir(directory);
var w = open("file.txt", ioMode.cwr).writer();
w.writeln("hello");
w.close();
