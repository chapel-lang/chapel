use Regexp;
use IO;

config type t = string;
writeln("RE TESTS");
var f = openmem();
var w = f.writer();
w.write("Baz9xzz");
w.close();

var r = f.reader();
var str:t;
var ok:bool;

writeln("#1 not match");
try {
  ok = r.readf("%/[a-z]/":t);
} catch err: BadFormatError {
  ok = false;
} catch err {
  writeln(err);
  halt();
}
writeln(r.offset(), " ", ok);
writeln("#2 passes B, should match");
ok = r.readf("%/[A-Z]/":t);
writeln(r.offset(), " ", ok);
writeln("#3 captures az, should match");
ok = r.readf("%/([a-z]+)/":t, str);
writeln(r.offset(), " ", ok, ":", str);
writeln("#4 should not match");

try {
  r.readf("%/(0-9)/":t, str);
  ok = true;
} catch {
  ok = false;
}
writeln(r.offset(), " ", ok);

writeln("#5 captures 9, should match");
ok = r.readf("%/([0-9])/":t, str);
writeln(r.offset(), " ", ok, ":", str);


// in the following two, the format strings to readf are hardcoded and string
// and bytes, respectively. One can match a bytes regex using a string format
// and vice versa
writeln("#6 passes x, should match");
{
  var re = compile("x":t);
  ok = r.readf("%/*/", re);
  writeln(r.offset(), " ", ok);
}
writeln("#7 captures zz, should match");
{
  var re = compile("(z+)":t);
  ok = r.readf(b"%/*/", re, str);
  writeln(r.offset(), " ", ok, ":", str);
}
