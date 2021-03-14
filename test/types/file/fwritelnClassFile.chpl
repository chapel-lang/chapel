use IO;

class phone {
  var name: string;
  var cell: int;
}

var myFile = open("_test_fwritelnClassFile.txt", iomode.cw).writer();

var smh: borrowed phone = new borrowed phone(name = "shannon", cell = 2065551212);

myFile.writeln(smh);

myFile.close();
