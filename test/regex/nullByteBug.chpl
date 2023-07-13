use Regex;

var myRegex = new regex(b"a+");

var str = b"oneatwo";

writef("%?\n", str.replaceAndCount(myRegex, b"\x00"));

writef("%?\n", (new regex(b"\x00")):bytes);
writef("%?\n", (new regex(b"a\x00")):bytes);
writef("%?\n", (new regex(b"\x00a")):bytes);

var r3 = new regex(b"\x00+");

writef("%?\n", r3:bytes);

writef("%?\n", b"a\x00b\x00\x00c\x00\x00\x00".replace(r3, b"A"));
