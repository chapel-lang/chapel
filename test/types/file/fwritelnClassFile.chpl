use IO;

class phone {
  var name: string;
  var cell: int;
}

var myFile = open("_test_fwritelnClassFile.txt", ioMode.cw).writer();

var ownSmh = new owned phone(name = "shannon", cell = 2065551212);
var smh: borrowed phone = ownSmh.borrow();

myFile.writeln(smh);

myFile.close();
