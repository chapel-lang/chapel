use IO;

class Parent {
}

class Child : Parent {
  var x: int;
  var y: int;
  var z: int;
}

var ownA = new owned Child(x = 1, y = 2, z = 3);
var a: borrowed Child = ownA.borrow();
var ownB = new owned Child(x = 10, y = 20, z = 30);
var b: borrowed Child = ownB.borrow();

writeln("a is ", a);
writeln("b is ", b);

var f = open("test.txt", ioMode.cwr);
var writer = f.writer();
writer.writeln(a);
writer.close();

var reader = f.reader();
reader.read(b);
writeln("b after reading in a's values from a file: ", b);
