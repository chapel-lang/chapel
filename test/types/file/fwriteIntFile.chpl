var myInt = 123;
var f = open("_test_fwriteIntFile.txt", mode.w).writer();

f.write(myInt);

f.close();
