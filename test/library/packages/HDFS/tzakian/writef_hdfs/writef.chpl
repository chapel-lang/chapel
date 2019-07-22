use HDFS;

var hdfs = hdfs_chapel_connect("default", 0);
var ff = hdfs.hdfs_chapel_open("/tmp/writef_test.txt", iomode.cw);
var ch = ff.writer();

ch.writeln("Part 1");

ch.writef("HELOOLO \n%{##.###}\n bla \n%{##}\n", 1.1234, 5.1);
ch.writef("HELLO");
ch.writef("HELLO\n");
ch.writef("%{#}\n", 5);


ch.writef("\n\n");

ch.writef("%%\n");
ch.writef("#\n");

ch.writeln("Part 2");
ch.writef("%xi\n", 17);
ch.writef("%xu\n", 17);
ch.writef("%bi\n", 3);
ch.writef("%bu\n", 3);
ch.writef("%di\n", 11);
ch.writef("%du\n", 11);

ch.writef("% 13i\n", 11);
ch.writef('%13"S\n', "bog\"o");
ch.writef("%xn\n", 11);

ch.writeln("Part 3");
ch.writef("%xr\n", 1.0);
//ch.writef("%bf\n", 2.0); not yet supported.
ch.writef("%xr\n", 17.2938);
//ch.writef("%xF\n", 17.2938);
ch.writef("%Xr\n", 1.02e27);
ch.writef("%Xr\n", 1.02e-27);

ch.writeln("Part 4");
ch.writef("%t\n", (1,17));
ch.writef("%xt\n", (1,17));
ch.writef("%@xt\n", (1,17));
ch.writef("%jt\n", (1,17));

ch.writeln("Part 5");
ch.writef("%*i\n", 17, 1);
ch.writef("%17.2dr\n", 21.01011);
ch.writef("%*.*dr\n", 17, 2, 21.01011);
ch.writef("%17.2r\n", 21.01011);
ch.writef("%*.*r\n", 17, 2, 21.01011);
ch.writef("%17.3r\n", 21.01011);
ch.writef("%*.*r\n", 17, 3, 21.01011);
ch.writef("%17.4r\n", 21.01011);
ch.writef("%*.*r\n", 17, 4, 21.01011);



ch.writeln("Part 6");
ch.writef("%*S\n", "A", "test");
ch.writef("%*S\n", 98, "test");
ch.writef("%*S\n", "'", "test");
ch.writef("%{(S)}\n", "test");

ch.writef("%|4s\n", "test");

ch.writeln("Part 7");
ch.writef("%@bi\n", -3);
ch.writef("%@bu\n", 3);
ch.writef("%@xi\n", -10);
ch.writef("%@xu\n", 10);

ch.writeln("Part 8");
ch.writef("|%07i|\n", 7);
ch.writef("|%-7i|\n", 7);
ch.writef("|%0-7i|\n", 7);
ch.writef("|% -7i|\n", 7);
ch.writef("|%0+7i|\n", 7);
ch.writef("|% +7i|\n", 7);

ch.writeln("From README");
ch.writef("%5i %5s %5r\n", 1, "test", 6.34);
ch.writef("%2.4z\n", 43.291 + 279.112i);
ch.writef('%"S\n', "test \"\" \'\' !");
ch.writef("%'S\n", "test \"\" \'\' !");
ch.writef("%{(S)}\n", "test ()");
ch.writef("%40s|\n", "test");
ch.writef("%-40s|\n", "test");

ch.writef("123456\n");
ch.writef("%6.6'S\n", "a");
ch.writef("%6.6'S\n", "abcdefg");
ch.writef("%.3'S\n", "a");
ch.writef("%.3'S\n", "abcd");

ch.close();
ff.close();
var r = hdfs.hdfs_chapel_open("/tmp/writef_test.txt", iomode.r);
var rr = r.reader();

var str:  string;
while(rr.readline(str)) {
  write(str);
}
rr.close();
r.close();
hdfs.hdfs_chapel_disconnect();
