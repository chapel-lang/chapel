var myFirstInt = 123;
var mySecondInt = 456;
var myThirdInt = 789;

var f = open("_test_fwriteMultipleInts.txt", mode.w).writer();
f.write(myFirstInt, mySecondInt, myThirdInt);
f.close();

