config type dataType = string;

var s = "01234":dataType;

writeln(s[..(-1):byteIndex] == "":dataType);
writeln(s[..(-2):byteIndex] == "":dataType);  // this used to be OOB

