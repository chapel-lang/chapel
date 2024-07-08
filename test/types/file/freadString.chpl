use IO;

var myFirstString: string = "This string should never print.";
var mySecondString: string;

var f = open("freadString.txt", ioMode.r).reader(locking=false);

f.read(myFirstString, mySecondString);
writeln(myFirstString, " ", mySecondString);
