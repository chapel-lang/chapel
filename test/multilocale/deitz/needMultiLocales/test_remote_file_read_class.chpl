use IO;

class C {
  var x: int;
  var y: real;
}

var w = open("test_remote_file_read_class.txt", iomode.cw).writer();

var c = new unmanaged C(x=1,y=2.3);

writeln(c);

w.writeln(c);
w.close();

var r = open("test_remote_file_read_class.txt", iomode.r).reader();

var d = new unmanaged C();

on Locales(1) {
  r.read(d);
  r.close();
}

writeln(d);

delete d;
delete c;
