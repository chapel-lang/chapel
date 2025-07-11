//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO.FormattedIO;

/* START_EXAMPLE */
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
/* STOP_EXAMPLE */
