var myFirstString: string = "This string should never print.";
var mySecondString: string;

var f: file = new file(filename = "freadString.txt", mode = FileAccessMode.read);
f.open();

f.read(myFirstString, mySecondString);
writeln(myFirstString, " ", mySecondString);
