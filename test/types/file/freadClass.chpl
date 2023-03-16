use IO;

class myClass {
  var x: int;
  var y: real;
}

var ownA = new owned myClass(x = 1, y = 2.3);
var a: borrowed myClass = ownA.borrow();
var ownB = new owned myClass(x = 9, y = 9.9);
var b: borrowed myClass = ownB.borrow();

writeln("a is initialized to: ", a);
writeln("b is initialized to: ", b);

var f = open("_test_freadClass.txt", ioMode.cwr);
var writer = f.writer();
writer.writeln(a);
writer.close();

var reader = f.reader();
reader.read(b);
writeln("b after reading in a's values from a file: ", b);
