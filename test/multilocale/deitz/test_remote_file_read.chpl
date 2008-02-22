var f = new file("test_remote_file_read.txt");
var i: int;

f.open();
f.readln(i);
f.close();
writeln(i);

i = 0;

on Locales(1) {
  f.open();
  f.readln(i);
  f.close();
}

writeln(i);
