var myFirstInt = 123;
var mySecondInt = 456;
var myThirdInt = 789;

var f: file = file(filename = "_test_fwriteMultipleInts.txt", mode = "w");
f.open;
fwrite(f, myFirstInt, mySecondInt, myThirdInt);
