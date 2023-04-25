use IO;

var style = defaultIOStyleInternal();
var f = openfd(1, style=style);
f.close();
