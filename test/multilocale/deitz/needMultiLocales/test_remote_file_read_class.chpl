use IO;

class C {
  var x: int;
  var y: real;
}

var w = open("test_remote_file_read_class.txt", ioMode.cw).writer(locking=true);

var c = new C(x=1,y=2.3);

writeln(c);

w.writeln(c);
w.close();

var r = open("test_remote_file_read_class.txt", ioMode.r).reader(locking=true);

var d = new C();

on Locales(1) {
  r.read(d);
  r.close();
}

writeln(d);
