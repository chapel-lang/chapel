use IO;

var myFirstInt = 123;
var mySecondInt = 456;
var myThirdInt = 789;

var f = open("_test_fwritelnIntFile.txt", ioMode.cw).writer();

f.writeln(myFirstInt);
f.writeln(mySecondInt);
f.writeln(myThirdInt);

f.close();

