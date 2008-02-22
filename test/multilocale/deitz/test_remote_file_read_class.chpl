class C {
  var x: int;
  var y: real;
}

var f = new file(filename="test_remote_file_read_class.txt", mode=FileAccessMode.write);

var c = new C(x=1,y=2.3);

writeln(c);

f.open();
f.writeln(c);
f.close();

f.mode = FileAccessMode.read;

var d: C;

on Locales(1) {
  f.open();
  f.read(d);
  f.close();
}

writeln(d);
