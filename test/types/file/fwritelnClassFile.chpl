class phone {
  var name: string;
  var cell: int;
}

var myFile: file = new file(filename = "_test_fwritelnClassFile.txt", mode = "w");
myFile.open();

var smh: phone = new phone(name = "shannon", cell = 2065551212);
myFile.writeln(smh);
