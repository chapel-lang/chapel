// NOTE:  This test prints a.x = 1 when it should print a.x = 9.
// shannon, finish futurizing this.  commit.

class myClass {
  var x: int;
  var y: real;
}

var a: unmanaged myClass = new unmanaged myClass(x = 1, y = 2.3);
var myInt: int = 9;
var myFile = open("_test_freadToClassDataMember.txt", iomode.cwr);
var w = myFile.writer();

w.writeln(myInt);
w.close();

var r = myFile.reader();
r.read(a.x);
r.close();

writeln("a.x should be equal to 9");
writeln("a.x = ", a.x);

delete a;
