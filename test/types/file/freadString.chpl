var myFirstString: string = "This string should never print.";
var mySecondString: string;

var f: file = file(filename = "freadString.txt", mode = "r");
f.open();

f.read(myFirstString, mySecondString);
writeln(myFirstString, " ", mySecondString);
