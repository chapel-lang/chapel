use IO;

var f = openTempFile();
var ch = f.reader();
var style = defaultIOStyleInternal();
var i: int;
ch.read(i, style=style);
ch.close();
f.close();
