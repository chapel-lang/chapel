writeln("Part 1");

writef("HELOOLO \n%{##.###}\n bla \n%{##}\n", 1.1234, 5.1);
writef("HELLO");
writef("HELLO\n");
writef("%{#}\n", 5);


writef("\n\n");

writef("%%\n");
writef("#\n");

writeln("Part 2");
writef("%xi\n", 17);
writef("%xu\n", 17);
writef("%bi\n", 3);
writef("%bu\n", 3);
writef("%di\n", 11);
writef("%du\n", 11);

writef("% 13i\n", 11);
writef('%13"S\n', "bog\"o");
writef("%xn\n", 11);

writeln("Part 3");
writef("%xr\n", 1.0);
//writef("%bf\n", 2.0); not yet supported.
writef("%xr\n", 17.2938);
//writef("%xF\n", 17.2938);
writef("%Xr\n", 1.02e27);
writef("%Xr\n", 1.02e-27);

writeln("Part 4");
writef("%t\n", (1,17));
writef("%xt\n", (1,17));
writef("%@xt\n", (1,17));
writef("%jt\n", (1,17));

writeln("Part 5");
writef("%*i\n", 17, 1);
writef("%17.2dr\n", 21.01011);
writef("%*.*dr\n", 17, 2, 21.01011);
writef("%17.2r\n", 21.01011);
writef("%*.*r\n", 17, 2, 21.01011);
writef("%17.3r\n", 21.01011);
writef("%*.*r\n", 17, 3, 21.01011);
writef("%17.4r\n", 21.01011);
writef("%*.*r\n", 17, 4, 21.01011);



writeln("Part 6");
writef("%*S\n", "A", "test");
writef("%*S\n", 98, "test");
writef("%*S\n", "'", "test");
writef("%{(S)}\n", "test");

writef("%|4s\n", "test");

writeln("Part 7");
writef("%@bi\n", -3);
writef("%@bu\n", 3);
writef("%@xi\n", -10);
writef("%@xu\n", 10);

writeln("Part 8");
writef("|%07i|\n", 7);
writef("|%-7i|\n", 7);
writef("|%0-7i|\n", 7);
writef("|% -7i|\n", 7);
writef("|%0+7i|\n", 7);
writef("|% +7i|\n", 7);

writeln("From README");
writef("%5i %5s %5r\n", 1, "test", 6.34);
writef("%2.4z\n", 43.291 + 279.112i);
writef('%"S\n', "test \"\" \'\' !");
writef("%'S\n", "test \"\" \'\' !");
writef("%{(S)}\n", "test ()");
writef("%40s|\n", "test");
writef("%-40s|\n", "test");

writef("123456\n");
writef("%6.6'S\n", "a");
writef("%6.6'S\n", "abcdefg");
writef("%.3'S\n", "a");
writef("%.3'S\n", "abcd");

