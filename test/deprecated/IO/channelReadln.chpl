use IO;

var f = opentmp();
var ch = f.reader();
var style = defaultIOStyleInternal();
var i: int;
ch.readln(i, style=style);
ch.close();
f.close();
