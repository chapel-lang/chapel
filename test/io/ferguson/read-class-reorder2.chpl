use IO;

class Base {
}

class Parent : Base {
  var a: int;
  var b: int;
}

class Child : Parent {
  var x: int;
  var y: int;
}

var a: borrowed Child = new borrowed Child(a = 1, b = 2, x = 3, y = 4);

writeln("a is ", a);

var f = open("test.txt", iomode.cwr);
var writer = f.writer();
var s = "{b=5,a=4,y=7,x=6}";
writer.writeln(s);
writeln("writing ", s);
writer.close();

var reader = f.reader();
reader.read(a);
writeln("a after reading is ", a);
