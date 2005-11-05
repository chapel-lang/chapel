var myFirstString: string = "This string should never print.";
var mySecondString: string;

var f: file = file(filename = "freadString.txt", mode = "r");
f.open;

fread(f, myFirstString, mySecondString);
writeln(myFirstString, " ", mySecondString);
