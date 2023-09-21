use IO;

var f = openTempFile();
var style = defaultIOStyleInternal();
var l = f.lines(local_style=style);
f.close();
