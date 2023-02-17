use Regex;

var myRegex = new regex(b"a+");

var str = b"oneatwo";

writef("%t\n", str.replaceAndCount(myRegex, b"\x00"));

writef("%t\n", (new regex(b"\x00")):bytes);
writef("%t\n", (new regex(b"a\x00")):bytes);
writef("%t\n", (new regex(b"\x00a")):bytes);

var r3 = new regex(b"\x00+");

writef("%t\n", r3:bytes);

writef("%t\n", b"a\x00b\x00\x00c\x00\x00\x00".replace(r3, b"A"));
