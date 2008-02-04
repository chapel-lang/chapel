var myFirstInt = 123;
var mySecondInt = 456;
var myThirdInt = 789;

var f: file = new file(filename = "_test_fwriteMultipleInts.txt", mode = "w");
f.open();
f.write(myFirstInt, mySecondInt, myThirdInt);
