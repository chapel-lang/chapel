var myInt = 123;
var f = open("_test_fwriteIntFile.txt", iomode.w).writer();

f.write(myInt);

f.close();
