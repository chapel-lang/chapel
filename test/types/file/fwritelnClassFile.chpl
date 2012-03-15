class phone {
  var name: string;
  var cell: int;
}

var myFile = open("_test_fwritelnClassFile.txt", iomode.cw).writer();

var smh: phone = new phone(name = "shannon", cell = 2065551212);
myFile.writeln(smh);

myFile.close();

