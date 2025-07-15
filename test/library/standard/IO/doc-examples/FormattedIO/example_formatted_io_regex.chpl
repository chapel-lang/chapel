//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO;
use Regex;

/* START_EXAMPLE_1 */
var got = readf("%/a+/");
// if the input is "a" or "aa" (and so on), got will return true
// if the input is "c", a "BadFormatError" is triggered
/* STOP_EXAMPLE_1 */

writeln("got = ", got); // test "a"
stdin.advance(1); // advance the offset by 1 to skip the delimiter character "b"
got = readf("%/a+/");
writeln("got = ", got); // test "aa"
try! {
  got = readf("%/a+/");
}
catch e:BadFormatError {
  writeln(e); // testing "c" will fail, so we catch the error
  stdin.advance(1); // advance offset by 1 to skip "c"
}

{
/* START_EXAMPLE_2 */
var s:string;
var got = readf("%/a(b+)/", s);
// if the input is "ab", got will be true and s will be "b"
// if the input is "abb", got will be true and s will be "bb"
// if the input is "c", a "BadFormatError" is triggered
/* STOP_EXAMPLE_2 */

  writeln("got = ", got, ", s = ", s); // test "ab"
  got = readf("%/a(b+)/", s);
  writeln("got = ", got, ", s = ", s); // test "abb"
  try! {
    got = readf("%/a(b+)/", s);
  }
  catch e:BadFormatError {
    writeln(e); // testing "c" will fail, so we catch the error
  }
}
