var myFirstInt = 123;
var mySecondInt = 456;
var myThirdInt = 789;

var f: file = file(filename = "_test_fwritelnIntFile.txt", mode = "w");

f.open;
fwriteln(f, myFirstInt);
fwriteln(f, mySecondInt);
fwriteln(f, myThirdInt);
