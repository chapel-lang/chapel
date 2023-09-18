use BigInteger;
use IO.FormattedIO;

config const executeLocale = 0;

var a: bigint = 17;

on Locales[min(Locales.domain.high, executeLocale)] {
  var s = a:string; // test cast in multilocal and local
  writeln(s);
  writeln(a); // test writeThis in multilocal and local
  writef("%?\n", a); // check formatted strings
  writeln("%?".format(a)); // check formatted strings
}
