class phone {
  var name: string;
  var cell: int;
}

var myFile: file = file(filename = "_test_fwritelnClassFile.txt", mode = "w");
myFile.open();

var smh: phone = phone(name = "shannon", cell = 2065551212);
myFile.writeln(smh);
