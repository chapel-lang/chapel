use Regex;
use IO, ChplFormat;

var myRegex = new regex(b"a+");

var str = b"oneatwo";

var chplout = stdout.withSerializer(chplSerializer);

chplout.writef("%?\n", str.replaceAndCount(myRegex, b"\x00"));

chplout.writef("%?\n", (new regex(b"\x00")):bytes);
chplout.writef("%?\n", (new regex(b"a\x00")):bytes);
chplout.writef("%?\n", (new regex(b"\x00a")):bytes);

var r3 = new regex(b"\x00+");

chplout.writef("%?\n", r3:bytes);

chplout.writef("%?\n", b"a\x00b\x00\x00c\x00\x00\x00".replace(r3, b"A"));
