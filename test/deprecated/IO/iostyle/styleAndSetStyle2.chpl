use IO;

var f = open("styleAndSetStyle2.txt", ioMode.r);
var ch = f.reader();
var s = ch._style();
ch._set_style(s);
ch.close();
f.close();
