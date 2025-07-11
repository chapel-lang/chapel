//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO;
use Regex;

/* START_EXAMPLE */
var got = readf("%/a+/");
// if the input is "a" or "aa" (and so on), got will return true
// if the input is "c", a "BadFormatError" is triggered
/* STOP_EXAMPLE */

writeln("got = ", got); // test "a"
stdin.advance(1); // advance the offset by 1 to skip the delimiter character "b"
got = readf("%/a+/");
writeln("got = ", got); // test "aa"
try! {
  got = readf("%/a+/");
}
catch e:BadFormatError {
  writeln(e); // testing "c" will fail, so we catch the error
}
