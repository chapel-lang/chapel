use IO;

var f = open("test_remote_file_read.txt", iomode.r).reader();
var i: int;

f.readln(i);
f.close();
writeln(i);

f = open("test_remote_file_read.txt", iomode.r).reader();

i = 0;

on Locales(1) {
  f.readln(i);
  f.close();
}

writeln(i);
