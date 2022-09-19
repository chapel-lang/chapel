use IO;

var f = open("blah2.txt", iomode.cw);
var ch = f.writer();
var s = ch._style();
ch._set_style(s);
ch.close();
f.close();
