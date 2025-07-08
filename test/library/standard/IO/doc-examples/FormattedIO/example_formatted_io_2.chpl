//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO;

/* START_EXAMPLE */
var s:string;
var got = readf(" %c", s);
// if the input is " a", "\na", "  a", etc, s will contain "a"
// if the input is "b", a "BadFormatError" is triggered
/* STOP_EXAMPLE */

writeln("got = ", got, ", s = ", s); // test " a"
got = readf(" %c", s);
writeln("got = ", got, ", s = ", s); // test "\na"
got = readf(" %c", s);
writeln("got = ", got, ", s = ", s); // test "  a"
try! {
  got = readf(" %c", s);
}
catch e:BadFormatError {
  writeln(e); // testing "b" will fail, so we catch the error
}
