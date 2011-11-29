var myInt = 123;
var f = open("fwriteIntFailed.txt", mode.r).writer();

f.write(myInt);
