//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO;

/* START_EXAMPLE_1 */
writef("%5i %5s %5r\n", 1, "test", 6.34);
     // outputs:
     //     1  test  6.34

writef("%2.4z\n", 43.291 + 279.112i);
     // outputs:
     // 43.29 + 279.1i

writef('%"S\n', "test \"\" \'\' !");
     // outputs:
     // "test \"\" '' !"
writef("%'S\n", "test \"\" \'\' !");
     // outputs:
     // 'test "" \'\' !'
writef("%{(S)}\n", "test ()");
     // outputs:
     // (test (\))

writef("|%40s|\n", "test");
writef("|%<40s|\n", "test");
writef("|%^40s|\n", "test");
writef("|%>40s|\n", "test");
     // outputs:
     // |                                    test|
     // |test                                    |
     // |                  test                  |
     // |                                    test|

writef("123456\n");
writef("%6.6'S\n", "a");
writef("%6.6'S\n", "abcdefg");
writef("%.3'S\n", "a");
writef("%.3'S\n", "abcd");
     // outputs:
     // 123456
     //    'a'
     // 'a'...
     // 'a'
     // ''...
/* STOP_EXAMPLE_1 */

{
/* START_EXAMPLE_2 */
var s:string;
var got = readf(" %c", s);
// if the input is " a", "\na", "  a", etc, s will contain "a"
// if the input is "b", a "BadFormatError" is triggered
/* STOP_EXAMPLE_2 */

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
    stdin.advance(1); // advance offset by 1 to skip "b"
  }
}

{
/* START_EXAMPLE_3 */
var s:string;
var got = readf("\n%c", s);
// if the input is "\na", or " \na", s will contain "a"
// if the input is "b", a "BadFormatError" is triggered
/* STOP_EXAMPLE_3 */

  writeln("got = ", got, ", s = ", s); // test "\na"
  got = readf("\n%c", s);
  writeln("got = ", got, ", s = ", s); // test " \na"
  try! {
    got = readf("\n%c", s);
  }
  catch e:BadFormatError {
    writeln(e); // testing "b" will fail, so we catch the error
  }
}
