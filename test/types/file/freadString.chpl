use IO;

var myFirstString: string = "This string should never print.";
var mySecondString: string;

var f = open("freadString.txt", ioMode.r).reader();

f.read(myFirstString, mySecondString);
writeln(myFirstString, " ", mySecondString);
