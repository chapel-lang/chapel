use Regex;

var myRegex = compile(b"a+");

var str = b"oneatwo";

writef("%t\n", myRegex.subn(b"\x00", str));

writef("%t\n", compile(b"\x00"):bytes);
writef("%t\n", compile(b"a\x00"):bytes);
writef("%t\n", compile(b"\x00a"):bytes);

var r3 = compile(b"\x00+");

writef("%t\n", r3:bytes);

writef("%t\n", r3.sub(b"A", b"a\x00b\x00\x00c\x00\x00\x00"));