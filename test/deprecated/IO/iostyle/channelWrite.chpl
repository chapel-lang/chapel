use IO;

var f = openTempFile();
var ch = f.writer();
var style = defaultIOStyleInternal();
var i: int;
ch.write(i, style=style);
ch.close();
f.close();
