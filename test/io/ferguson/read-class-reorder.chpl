use IO;

/*class Base {
}

class Parent : Base {
  var x: int;
  var y: int;
}
*/

class Child /*: Parent*/ {
  var x: int;
  var y: int;
  var z: int;
}

var a: borrowed Child = new borrowed Child(x = 1, y = 2, z = 3);

writeln("a is ", a);

var f = open("test.txt", iomode.cwr);
var writer = f.writer();
var s = "{z=6,y=5,x=4}";
writer.writeln(s);
writeln("writing ", s);
writer.close();

var reader = f.reader();
reader.read(a);
writeln("a after reading is ", a);
