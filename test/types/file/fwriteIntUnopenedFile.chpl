var myInt = 123;
var f: file = file(filename = "unopenedFile.txt", mode = "w");

var s: string = myInt;
fwrite(f, s);
