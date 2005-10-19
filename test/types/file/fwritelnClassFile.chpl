class phone {
  var name: string;
  var cell: integer;
}

var myFile: file = file(filename = "_test_fwritelnClassFile.txt", mode = "w");
myFile.open;

var smh: phone = phone(name = "shannon", cell = 2063833241);
fwriteln(myFile, smh);
