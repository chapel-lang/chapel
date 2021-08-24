use Regex;

var myRegex = compile(b"a+");

var str = b"oneatwo";

writeln(myRegex.subn(b"\x00", str));

