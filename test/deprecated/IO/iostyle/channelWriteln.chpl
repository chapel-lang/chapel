use IO;

var f = openTempFile();
var ch = f.writer();
var style = defaultIOStyleInternal();
var i: int;
ch.writeln(i, style=style);
ch.close();
f.close();
