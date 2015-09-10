writeln("RE TESTS");
var f = openmem();
var w = f.writer();
w.write("Baz9xzz");
w.close();

var r = f.reader();
var str:string;
var ok:bool;
var error:syserr;

writeln("#1 not match");
ok = r.readf("%/[a-z]/");
writeln(r.offset(), " ", ok);
writeln("#2 passes B, should match");
ok = r.readf("%/[A-Z]/");
writeln(r.offset(), " ", ok);
writeln("#3 captures az, should match");
ok = r.readf("%/([a-z]+)/", str);
writeln(r.offset(), " ", ok, ":", str);
writeln("#4 should not match");
ok = r.readf("%/(0-9)/", str, error=error);
writeln(r.offset(), " ", ok);
writeln("#5 captures 9, should match");
ok = r.readf("%/([0-9])/", str);
writeln(r.offset(), " ", ok, ":", str);
writeln("#6 passes x, should match");
{
  var re = compile("x");
  ok = r.readf("%/*/", re);
  writeln(r.offset(), " ", ok);
}
writeln("#7 captures zz, should match");
{
  var re = compile("(z+)");
  ok = r.readf("%/*/", re, str);
  writeln(r.offset(), " ", ok, ":", str);
}
