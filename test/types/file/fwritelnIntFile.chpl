var myFirstInt = 123;
var mySecondInt = 456;
var myThirdInt = 789;

var f: file = new file(filename = "_test_fwritelnIntFile.txt", mode = FileAccessMode.write);

f.open();
f.writeln(myFirstInt);
f.writeln(mySecondInt);
f.writeln(myThirdInt);
