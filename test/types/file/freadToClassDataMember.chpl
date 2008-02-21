// NOTE:  This test prints a.x = 1 when it should print a.x = 9.
// shannon, finish futurizing this.  commit.

class myClass {
  var x: int;
  var y: real;
}

var a: myClass = new myClass(x = 1, y = 2.3);
var myInt: int = 9;
var myFile: file = new file(filename = "_test_freadToClassDataMember.txt", mode = FileAccessMode.write);

myFile.open();
myFile.writeln(myInt);
myFile.close();

myFile.mode = FileAccessMode.read;
myFile.open();
myFile.read(a.x);
myFile.close();

writeln("a.x should be equal to 9");
writeln("a.x = ", a.x);

