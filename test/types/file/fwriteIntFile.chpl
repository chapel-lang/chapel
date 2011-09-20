var myInt = 123;
var f = open("_test_fwriteIntFile.txt", "w").writer();

f.write(myInt);

f.close();
