class myClass {
  var x: int;
  var y: real;
}

var myFile: file = new file(filename = "_test_freadClass.txt", mode = FileAccessMode.write);
var a: myClass = new myClass(x = 1, y = 2.3);
var b: myClass = new myClass(x = 9, y = 9.9);

writeln("a is initialized to: ", a);
writeln("b is initialized to: ", b);

myFile.open();
myFile.writeln(a);
myFile.close();

myFile.mode = FileAccessMode.read;
myFile.open();
myFile.read(b);
writeln("b after reading in a's values from a file: ", b);
