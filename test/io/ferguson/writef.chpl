pragma "error mode fatal"
module test {
  config type t = string;
  writeln("Part 1");

  writef("HELOOLO \n%{##.###}\n bla \n%{##}\n":t, 1.1234, 5.1);
  writef("HELLO");
  writef("HELLO\n");
  writef("%{#}\n":t, 5);


  writef("\n\n");

  writef("%%\n");
  writef("#\n");

  writeln("Part 2");
  writef("%xi\n":t, 17);
  writef("%xu\n":t, 17);
  writef("%bi\n":t, 3);
  writef("%bu\n":t, 3);
  writef("%di\n":t, 11);
  writef("%du\n":t, 11);

  writef("% 13i\n":t, 11);
  writef('%13"S\n', "bog\"o");
  writef("%xn\n":t, 11);

  writeln("Part 3");
  writef("%xr\n":t, 1.0);
  //writef("%bf\n":t, 2.0); not yet supported.
  writef("%xr\n":t, 17.2938);
  //writef("%xF\n":t, 17.2938);
  writef("%Xr\n":t, 1.02e27);
  writef("%Xr\n":t, 1.02e-27);

  writeln("Part 4");
  writef("%t\n":t, (1,17));
  writef("%xt\n":t, (1,17));
  writef("%@xt\n":t, (1,17));
  writef("%jt\n":t, (1,17));

  writeln("Part 5");
  writef("%*i\n":t, 17, 1);
  writef("%17.2dr\n":t, 21.01011);
  writef("%*.*dr\n":t, 17, 2, 21.01011);
  writef("%17.2r\n":t, 21.01011);
  writef("%*.*r\n":t, 17, 2, 21.01011);
  writef("%17.3r\n":t, 21.01011);
  writef("%*.*r\n":t, 17, 3, 21.01011);
  writef("%17.4r\n":t, 21.01011);
  writef("%*.*r\n":t, 17, 4, 21.01011);



  writeln("Part 6");
  writef("%*S\n":t, "A", "test");
  writef("%*S\n":t, 98, "test");
  writef("%*S\n":t, "'", "test");
  writef("%{(S)}\n":t, "test");

  writef("%|4s\n":t, "test");

  writeln("Part 7");
  writef("%@bi\n":t, -3);
  writef("%@bu\n":t, 3);
  writef("%@xi\n":t, -10);
  writef("%@xu\n":t, 10);

  writeln("Part 8");
  writef("|%07i|\n":t, 7);
  writef("|%-7i|\n":t, 7);
  writef("|%0-7i|\n":t, 7);
  writef("|% -7i|\n":t, 7);
  writef("|%0+7i|\n":t, 7);
  writef("|% +7i|\n":t, 7);

  writeln("From README");
  writef("%5i %5s %5r\n":t, 1, "test", 6.34);
  writef("%2.4z\n":t, 43.291 + 279.112i);
  writef('%"S\n', "test \"\" \'\' !");
  writef("%'S\n":t, "test \"\" \'\' !");
  writef("%{(S)}\n":t, "test ()");
  writef("%40s|\n":t, "test");
  writef("%-40s|\n":t, "test");

  writef("123456\n");
  writef("%6.6'S\n":t, "a");
  writef("%6.6'S\n":t, "abcdefg");
  writef("%.3'S\n":t, "a");
  writef("%.3'S\n":t, "abcd");
}
