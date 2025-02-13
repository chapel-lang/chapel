use IO;

proc test(writer, args) {
  writer.write((...args));
  writer.writeln();
}
proc testln(writer, args) {
  writer.writeln((...args));
}
proc test(writer, args, sep) {
  writer.write((...args), sep=sep);
  writer.writeln();
}
proc testln(writer, args, sep) {
  writer.writeln((...args), sep=sep);
}

writeln("test with no sep");
test(stdout, (1, 2, 3));
writeln("testln with no sep");
testln(stdout, (4, 5, 6));
writeln("test with sep");
test(stdout, (1.0, "2", true), sep=", ");
writeln("testln with sep");
testln(stdout, (4.0, "5", false), sep=", ");


record R {
  var x: int;
  var y: int;
}
class C {
  var a: real;
  var b: real;
  var c: R;
}


writeln("testln with record with no sep");
testln(stdout, (new R(1, 2), new R(3, 4)));
writeln("testln with record with sep");
testln(stdout, (new R(1, 2), new R(3, 4)), sep=";");

writeln("testln with class with no sep");
testln(stdout, (new C(a=1.0, b=2.0, c=new R(3, 4)), new C(a=5.0, b=6.0, c=new R(7, 8))));
writeln("testln with class with sep");
testln(stdout, (new C(a=1.0, b=2.0, c=new R(3, 4)), new C(a=5.0, b=6.0, c=new R(7, 8))), sep="\n");
