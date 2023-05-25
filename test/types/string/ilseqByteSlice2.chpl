var s = "événement";

// I want to find "v", and get the slice that starts two characters after
writeln(s[s.find("v")+2..]);  // OK this prints "nement" nicely

// But we did byte-based slicing, so actually grabbed a junk byte from "é"
writeln(s[s.find("v")+2..].numBytes);  // prints 7 not 6
