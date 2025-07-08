//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO;
use Regex;

/* START_EXAMPLE */
var s:string;
var got = readf("%/a(b+)/", s);
// if the input is "ab", got will be true and s will be "b"
// if the input is "abb", got will be true and s will be "bb"
// if the input is "c", a "BadFormatError" is triggered
/* STOP_EXAMPLE */

writeln("got = ", got, ", s = ", s); // test "ab"
got = readf("%/a(b+)/", s);
writeln("got = ", got, ", s = ", s); // test "abb"
try! {
  got = readf("%/a(b+)/", s);
}
catch e:BadFormatError {
  writeln(e); // testing "c" will fail, so we catch the error
}
