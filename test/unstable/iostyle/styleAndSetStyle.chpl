use IO;

var f = open("blah2.txt", ioMode.cw);
var ch = f.writer();
var s = ch._style();
ch._set_style(s);
ch.close();
f.close();
